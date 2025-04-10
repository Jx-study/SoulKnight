//
// Created by QuzzS on 2025/3/7.
//

#ifndef ROOM_HPP
#define ROOM_HPP

#include "Camera.hpp"
#include "Components/CollisionComponent.hpp"
#include "Creature/Character.hpp"
#include "Factory/RoomObjectFactory.hpp"
#include "Override/nGameObject.hpp"
#include "RoomCollisionManager.hpp"
#include "RoomInteractionManager.hpp"
#include "RoomObject/RoomObject.hpp"
#include "pch.hpp"

// 房间状态枚举
enum class RoomState {
    INACTIVE,   // 未激活（玩家不在该房间）
    ACTIVE,     // 已激活（玩家在该房间内）
    COMBAT,     // 战斗中
    CLEARED,    // 已清理（战斗结束或无战斗房间）
    SPECIAL     // 特殊状态（宝箱房、商店等）
};

// 前向声明
class RoomCollisionManager;
class Camera;

class Room {
public:
    Room();
    virtual ~Room();

    // 核心方法
    virtual void Start(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Character>& player); // 房间初始化
    virtual void Update(); // 更新房间内所有对象

    // 房间状态管理
    virtual void SetState(RoomState state);
    [[nodiscard]] RoomState GetState() const { return m_State; }

    // 角色管理方法
    virtual void CharacterEnter(std::shared_ptr<Character> character);
    virtual void CharacterExit(std::shared_ptr<Character> character);
    [[nodiscard]] bool HasCharacter(const std::shared_ptr<Character>& character) const;
    [[nodiscard]] const std::vector<std::shared_ptr<Character>>& GetCharacters() const { return m_Characters; }

    // 房间对象管理
    void AddRoomObject(const std::shared_ptr<RoomObject>& object);
    void RemoveRoomObject(const std::shared_ptr<RoomObject>& object);
    [[nodiscard]] const std::vector<std::shared_ptr<RoomObject>>& GetRoomObjects() const { return m_RoomObjects; }

    // 碰撞体管理
    [[nodiscard]] std::shared_ptr<RoomCollisionManager> GetCollisionManager() const { return m_CollisionManager; }
    [[nodiscard]] std::shared_ptr<RoomInteractionManager> GetInteractionManager() const { return m_InteractionManager; }

    // Getter/Setter
    [[nodiscard]] glm::vec2 GetWorldCoord() const { return m_WorldCoord; }
    void SetWorldCoord(const glm::vec2& worldCoord) { m_WorldCoord = worldCoord; }

    [[nodiscard]] glm::vec2 GetSize() const { return m_Size; }
    void SetSize(const glm::vec2& size) { m_Size = size; }

    [[nodiscard]] glm::vec2 GetTileSize() const { return m_TileSize; }
    void SetTileSize(const glm::vec2& tileSize) { m_TileSize = tileSize; }

    [[nodiscard]] float GetRoomHeight() const { return m_RoomHeight; }
    void SetRoomHeight(float height) { m_RoomHeight = height; }

	void SetPlayer(const std::shared_ptr<Character>& player) { m_Player = player; }

    // 加载JSON配置
    virtual void LoadFromJSON(const std::string& jsonFilePath);

protected:
    // 房间状态
    RoomState m_State = RoomState::INACTIVE;

    // 房间属性
    glm::vec2 m_WorldCoord = glm::vec2(0, 0);  // 在世界中的位置
    glm::vec2 m_Size = glm::vec2(0, 0);        // 房间尺寸
    glm::vec2 m_TileSize = glm::vec2(0, 0);    // 瓦片尺寸
    float m_RoomHeight = 0.0f;                 // 房间高度

    // 房间内对象
    std::vector<std::shared_ptr<RoomObject>> m_RoomObjects;       // 房间固定物体
    std::vector<std::shared_ptr<Character>> m_Characters;         // 当前在房间内的角色

    // 工厂与管理器
    std::shared_ptr<RoomObjectFactory> m_Factory = std::make_shared<RoomObjectFactory>();
    std::shared_ptr<RoomCollisionManager> m_CollisionManager = std::make_shared<RoomCollisionManager>();
	std::shared_ptr<RoomInteractionManager> m_InteractionManager = std::make_shared<RoomInteractionManager>();

    // 相机引用
    std::weak_ptr<Camera> m_Camera;
	std::weak_ptr<Character> m_Player;

    // 子类需要实现的方法      // 设置墙壁碰撞体
    virtual void OnStateChanged(RoomState oldState, RoomState newState) {}  // 房间状态变化时调用

    // 更新房间状态的辅助方法
    virtual void UpdateRoomState();

    // 处理角色进入/离开房间时的事件 TODO:怎麽處理
    virtual void OnCharacterEnter(const std::shared_ptr<Character>& character) {}
    virtual void OnCharacterExit(const std::shared_ptr<Character>& character) {}
};

#endif //ROOM_HPP