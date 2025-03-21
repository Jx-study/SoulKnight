#include <utility>

//
// Created by QuzzS on 2025/3/15.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

//Component.hpp

struct CollisionInfo;
class nGameObject;

class Component
{
public:
	Component() = default;
	explicit Component(std::string name): m_Name(std::move(name)) {}
	virtual ~Component() = default;

	virtual void Init() {} //nGameObject在AddComponent就會自動執行 -- nGameObject.inl
	virtual void Update() {} //n
	virtual void HandleCollision(CollisionInfo& info) {}

	void SetOwner(const std::shared_ptr<nGameObject> &owner) { m_Owner = owner; } //nGameObject在AddComponent就會自動執行 -- nGameObject.inl
	[[nodiscard]] std::shared_ptr<nGameObject> GetOwner() const { return m_Owner.lock(); }
private:
	std::weak_ptr<nGameObject> m_Owner; // 打破循環引用,只能用GetOwner取得std::shared_ptr
	std::string m_Name = "None"; // 區別Component 比如hitbox和collision_box 方便閲讀
};

#endif //COMPONENT_HPP
