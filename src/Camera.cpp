//
// Created by QuzzS on 2025/2/28.
//

#include "Camera.hpp"

Camera::Camera(
	const std::vector<std::shared_ptr<nGameObject>> &pivotChildren
	) : m_RelativePivotChildren(pivotChildren)
{
	m_CameraWorldCoord.translation = {0.0f,0.0f};
	m_CameraWorldCoord.rotation = 0.0f;
	m_CameraWorldCoord.scale = {1.0f,1.0f};
}

void Camera::onInputReceived(const std::set<char>& keys) {
	if (keys.count('I')) ZoomCamera(1);
	if (keys.count('K')) ZoomCamera(-1);
}

// void Camera::MoveCamera(const glm::vec2 &displacement)
// {
// 	//加位移變化量
// 	m_CameraWorldCoord.translation += displacement * Util::Time::GetDeltaTimeMs() / 5.0f / std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
// }

void Camera::MoveCamera(const glm::vec2 &deltaDisplacement)
{
	m_CameraWorldCoord.translation += deltaDisplacement;
	// TODO:限制範圍 應該要圓形的 可能需要變數 範圍還沒限制到
	// const float u = beaconCoord.x * std::sqrt(1 - beaconCoord.y * beaconCoord.y / 2);
	// const float v = beaconCoord.y * std::sqrt(1 - beaconCoord.x * beaconCoord.x / 2);
	// if (m_CameraWorldCoord.translation.x > beaconCoord.x + u) {m_CameraWorldCoord.translation.x = beaconCoord.x + u;}
	// if (m_CameraWorldCoord.translation.y > beaconCoord.y + v) {m_CameraWorldCoord.translation.y = beaconCoord.y + v;}
	// if (m_CameraWorldCoord.translation.x < beaconCoord.x + u) {m_CameraWorldCoord.translation.x = beaconCoord.x + u;}
	// if (m_CameraWorldCoord.translation.y < beaconCoord.y + v) {m_CameraWorldCoord.translation.y = beaconCoord.y + v;}
}

void Camera::SetFollowTarget(const std::shared_ptr<nGameObject> &target) {
	m_FollowTarget = target;
}

void Camera::ZoomCamera(const float zoomLevel)
{
	const float minZoom = 0.5f;  // 最小縮放值
	const float maxZoom = 2.0f;  // 最大縮放值

	glm::vec2 newScale = m_CameraWorldCoord.scale + glm::vec2(1.0f, 1.0f) * zoomLevel * (Util::Time::GetDeltaTimeMs()/1000.0f);
	newScale.x = std::clamp(newScale.x, minZoom, maxZoom);
	newScale.y = std::clamp(newScale.y, minZoom, maxZoom);

	m_CameraWorldCoord.scale = newScale;
}

void Camera::RotateCamera(const float radian) // radian 是PI， degree是°
{
	m_CameraWorldCoord.rotation -= radian * Util::Time::GetDeltaTimeMs() / 1000;
}

void Camera::AddRelativePivotChild(const std::shared_ptr<nGameObject> &child) {
	m_RelativePivotChildren.push_back(child);
	// 如果尚未設置初始縮放
	if (!child->isSetInitialScale()) {
		child->SetInitialScale(child->m_Transform.scale);
		child->SetInitialScaleSet(true);
	}
}

void Camera::RemoveRelativePivotChild(const std::shared_ptr<nGameObject>& child) {
	m_RelativePivotChildren.erase(
		std::remove(m_RelativePivotChildren.begin(), m_RelativePivotChildren.end(), child),
		m_RelativePivotChildren.end()
	);
}

void Camera::AddRelativePivotChildren(
		const std::vector<std::shared_ptr<nGameObject>> &children) {
	m_RelativePivotChildren.reserve(m_RelativePivotChildren.size() + children.size());
	m_RelativePivotChildren.insert(m_RelativePivotChildren.end(), children.begin(), children.end());
}

//感覺可以優化 在渲染前一次性修改
void Camera::Update() {
	if (auto target = m_FollowTarget.lock()) {
		m_CameraWorldCoord.translation = target->m_WorldCoord;
	}

	for (const auto &child:m_RelativePivotChildren)
	{
		//變更坐標軸
		// child->SetPivot(m_CameraWorldCoord.translation - child->m_WorldCoord);//成功 - 跟著鏡頭縮放旋轉 但是改變Object Pivot以後槍旋轉點、子彈從槍口發射可能會有問題
		//Obejct窗口位置 = (Object世界坐標 - Camera世界坐標) * 縮放倍率
		child->m_Transform.translation = (child->m_WorldCoord - m_CameraWorldCoord.translation) * m_CameraWorldCoord.scale;

		if (m_MapSize != 0.0f)
		{
			child->SetZIndex((child->m_WorldCoord.y - child->GetImageSize().y/2.0f + m_MapSize/2.0f) / m_MapSize);
		}
		glm::vec2 initialScale = child->GetInitialScale();
		// std::copysign(第一個參數：大小, 第二個參數：正負號)
		child->m_Transform.scale = glm::vec2(
		    initialScale.x * std::copysign(m_CameraWorldCoord.scale.x, child->m_Transform.scale.x),
		    initialScale.y * std::copysign(m_CameraWorldCoord.scale.y, child->m_Transform.scale.y)
		);
	}
}



