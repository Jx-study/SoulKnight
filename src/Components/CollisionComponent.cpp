//
// Created by QuzzS on 2025/3/15.
//

// CollisionComponent.cpp

#include "Components/CollisionComponent.hpp"
#include "TriggerStrategy/SpeedZoneStrategy.hpp"
#include "Util/Logger.hpp"

#include "Structs/CircleShape.hpp"
#include "Structs/RectShape.hpp"
#include "Util/Image.hpp"
#include "spdlog/fmt/bundled/chrono.h"


void CollisionComponent::Init()
{
	auto owner = GetOwner<nGameObject>();
	if (!owner)
		return;

	// 创建 TriggerStrategy
	if (m_IsTrigger && !m_TriggerStrategyType.empty())
	{
		if (m_TriggerStrategyType == "SpeedZone")
		{
			try
			{
				float speedMultiplier = m_TriggerStrategyConfig.value("speedMultiplier", 1.0f);
				float duration = m_TriggerStrategyConfig.value("duration", 1.0f);
				AddTriggerStrategy(std::make_unique<SpeedZoneStrategy>(speedMultiplier, duration));
			}
			catch (const std::exception &e)
			{
				LOG_ERROR("Failed to create SpeedZoneStrategy: {}", e.what());
			}
		}
	}
}

void CollisionComponent::Update() {}

Rect CollisionComponent::GetBounds() const
{
	glm::vec2 objectPosition = {0.0f, 0.0f};
	auto owner = GetOwner<nGameObject>();
	if (owner)
	{
		objectPosition = owner->m_WorldCoord;
		glm::vec2 adjustedOffset = m_Offset;

		if (owner->m_Transform.scale.x < 0.0f)
		{
			adjustedOffset.x = -adjustedOffset.x; // 如果角色反向，X轴偏移需要镜像
		}
		return {objectPosition + adjustedOffset, m_Size};
	}
	return {objectPosition + m_Offset, m_Size};
}

std::shared_ptr<AreaShape> CollisionComponent::GetColliderAreaShape() const
{
	glm::vec2 objectPosition = {0.0f, 0.0f};
	if (const std::shared_ptr<nGameObject> owner = GetOwner<nGameObject>())
	{
		objectPosition = owner->GetWorldCoord();
		glm::vec2 adjustedOffset = m_Offset;
		// 反向處理
		if (owner->m_Transform.scale.x < 0.0f)
		{
			adjustedOffset.x = -adjustedOffset.x; // 如果角色反向，X轴偏移需要镜像
		}
		m_ColliderAreaShape->SetCenter(objectPosition + adjustedOffset);
	}
	m_ColliderAreaShape->SetCenter(objectPosition + m_Offset);
	return m_ColliderAreaShape;
}

std::shared_ptr<AreaShape> CollisionComponent::GetTriggerAreaShape() const
{
	glm::vec2 objectPosition = {0.0f, 0.0f};
	if (const std::shared_ptr<nGameObject> owner = GetOwner<nGameObject>())
	{
		objectPosition = owner->GetWorldCoord();
		glm::vec2 adjustedOffset = m_Offset;
		// 反向處理
		if (owner->m_Transform.scale.x < 0.0f)
		{
			adjustedOffset.x = -adjustedOffset.x; // 如果角色反向，X轴偏移需要镜像
		}
		m_TriggerAreaShape->SetCenter(objectPosition + adjustedOffset);
	}
	m_TriggerAreaShape->SetCenter(objectPosition + m_Offset);
	return m_TriggerAreaShape;
}

bool CollisionComponent::CanCollideWith(const std::shared_ptr<CollisionComponent> &other) const
{
	return m_CollisionMask & other->m_CollisionLayer; // 你的"圖層"在我的判定"範圍"之上， 所以我可以侵犯你(?
}

void CollisionComponent::HandleEvent(const EventInfo &eventInfo)
{
	// LOG_DEBUG("OnEvent3");
	if (eventInfo.GetEventType() == EventType::Collision)
	{
		// LOG_DEBUG("OnEvent4");
		const auto &collision = dynamic_cast<const CollisionEventInfo &>(eventInfo);
		// 碰撞變色功能已移除
		// SetColliderBoxColor("Yellow"); //碰撞變色
	}
}

std::vector<EventType> CollisionComponent::SubscribedEventTypes() const
{
	return {
		EventType::Collision,
	};
}

void CollisionComponent::AddTriggerStrategy(std::unique_ptr<ITriggerStrategy> strategy)
{
	m_TriggerStrategies.push_back(std::move(strategy));
}

void CollisionComponent::ClearTriggerStrategies() { m_TriggerStrategies.clear(); }

void CollisionComponent::ClearTriggerTargets()
{
	m_PreviousTriggerTargets.clear();
	m_CurrentTriggerTargets.clear();
}

void CollisionComponent::TryTrigger(const std::shared_ptr<nGameObject> &self, const std::shared_ptr<nGameObject> &other)
{
	if (!m_IsTrigger || m_TriggerStrategies.empty())
		return;
	// 如果同一幀已經處理過 other，就跳過
	auto [it, inserted] = m_CurrentTriggerTargets.insert(other);
	if (!inserted)
		return;

	bool wasTriggered = m_PreviousTriggerTargets.find(other) != m_PreviousTriggerTargets.end();
	for (auto &strategy : m_TriggerStrategies)
	{
		if (!wasTriggered)
			strategy->OnTriggerEnter(self, other);
		else
		{
			strategy->OnTriggerStay(self, other);
		}
	}
}

void CollisionComponent::FinishTriggerFrame(const std::shared_ptr<nGameObject> &self)
{
	if (!m_IsTrigger || m_TriggerStrategies.empty())
		return;

	// 對上一幀有觸發但本幀已經沒有的 other，觸發 OnTriggerExit
	for (auto &prev : m_PreviousTriggerTargets)
	{
		if (m_CurrentTriggerTargets.find(prev) == m_CurrentTriggerTargets.end())
		{
			for (auto &strategy : m_TriggerStrategies)
			{
				strategy->OnTriggerExit(self, prev);
			}
		}
	}

	// 交換集合：上一幀 ← 本幀，然後清空本幀集合
	std::swap(m_PreviousTriggerTargets, m_CurrentTriggerTargets);
	m_CurrentTriggerTargets.clear();
}
