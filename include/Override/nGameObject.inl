//
// Created by QuzzS on 2025/3/15.
//

// nGameObject.inl

#ifndef NGAMEOBJECT_INL
#define NGAMEOBJECT_INL

#include "nGameObject.hpp"

template <typename T, typename... Args>
std::shared_ptr<T> nGameObject::AddComponent(Args &&...args)
{
	//確保T是Component子類別的安全判斷，如果不用樣板直接用Component，那每次使用Function就要手動ObjectCast了
	static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
	auto component = std::make_shared<T>(std::forward<Args>(args)...);
	component->SetOwner(shared_from_this());
	m_Components.push_back(component);
	component->Init();
	return component;
}

template <typename T>
std::shared_ptr<T> nGameObject::GetComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
	for (auto &component : m_Components)
	{
		if (auto castedComponent = std::dynamic_pointer_cast<T>(component))
		{
			return castedComponent;
		}
	}
	return nullptr;
}


#endif //NGAMEOBJECT_INL
