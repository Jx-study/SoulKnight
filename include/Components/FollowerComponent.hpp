//
// Created by QuzzS on 2025/3/18.
//

// FollowerComponent.hpp

#ifndef FOLLOWERCOMPONENT_HPP
#define FOLLOWERCOMPONENT_HPP

#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "Components/Component.hpp"

class FollowerComponent final : public Component
{
public:
	explicit FollowerComponent(const ComponentType type = ComponentType::FOLLOWER,
							   const glm::vec2 &handOffset = glm::vec2(0, 0),
							   const glm::vec2 &holdingOffset = glm::vec2(0, 0), const float holdingRotation = 0.0f,
							   const bool isTargetMouse = false,
							   const std::shared_ptr<nGameObject> &follower = nullptr,
							   const std::shared_ptr<nGameObject> &target = nullptr) :
		Component(type), m_HandOffset(handOffset), m_HoldingOffset(holdingOffset),
		m_HoldingRotation(holdingRotation), m_UseMousePosition(isTargetMouse), m_Follower(follower),
		m_Target(target)
	{
	}

	~FollowerComponent() override = default;

	void BaseTargetRotate();
	void Update() override;


	//----Getter----
	[[nodiscard]] std::shared_ptr<nGameObject> GetFollower() const { return m_Follower.lock(); }
	[[nodiscard]] std::shared_ptr<nGameObject> GetTarget() const { return m_Target.lock(); }
	[[nodiscard]] float GetBaseRotation() const { return m_baseRotation; }
	[[nodiscard]] float GetHoldingRotation() const { return m_HoldingRotation; }
	[[nodiscard]] bool GetUseMouse() const { return m_UseMousePosition; }
	[[nodiscard]] float GetZIndexOffset() const { return m_ZIndexOffset; }
	[[nodiscard]] float GetBaseRotationDegrees() const { return m_baseRotationDegrees; }

	// Setter
	void SetFollower(const std::shared_ptr<nGameObject> &follower) { m_Follower = follower; }
	void SetTarget(const std::shared_ptr<nGameObject> &target) { m_Target = target; }
	void SetHandOffset(const glm::vec2 &handOffset) { m_HandOffset = handOffset; }
	void SetHoldingPosition(const glm::vec2 &holdingOffset) { m_HoldingOffset = holdingOffset; }
	void SetTargetMouse(const bool isTargetMouse) { m_UseMousePosition = isTargetMouse; }
	void SetIsSword(const bool isSword) { m_IsSword = isSword; }
	// void SetHoldingRotation(const float& rotation) {m_HoldingRotation = rotation;}
	void SetRotationLimits(float min, float max, bool enable = true)
	{
		m_RotationLimits = glm::vec2(min, max);
		m_EnableRotationLimits = enable;
	}
	void SetZIndexOffset(const float zIndexOffset) { m_ZIndexOffset = zIndexOffset; }
	void StartAttackAction();
	// 启用/禁用旋转限制
	void EnableRotationLimits(bool enable) { m_EnableRotationLimits = enable; }

protected:
	glm::vec2 m_RotationLimits = glm::vec2(-glm::pi<float>(), glm::pi<float>()); // 旋转限制 (min, max)
	bool m_EnableRotationLimits = false; // 是否启用旋转限制
	glm::vec2 m_HandOffset = glm::vec2(0, 0); //不完全以世界坐標為Pivot,有一點Offset
	glm::vec2 m_HoldingOffset = glm::vec2(0, 0); //武器也是
	float m_ZIndexOffset = 0.0f;
	float m_baseRotation = 0.0f;
	float m_HoldingRotation = 0;
	bool m_UseMousePosition = false;
	bool m_IsSword = false;
	bool m_attackAction = false;
	float m_attackTimer = 0.0f;
	float m_attackDuration = 0.4f;
	float m_baseRotationDegrees = 35.0f;	// 揮刀前的擺角
	std::weak_ptr<nGameObject> m_Follower;	// 此物件會跟隨的物件
	std::weak_ptr<nGameObject> m_Target;	// 此物件會跟隨目標旋轉
};


#endif //FOLLOWERCOMPONENT_HPP
