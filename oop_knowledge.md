# 元氣騎士 (Soul Knight) OOP 知識分析文檔

## 目錄
- [專案概述](#專案概述)
- [一、已運用的 OOP 核心概念](#一已運用的-oop-核心概念)
  - [1.1 繼承 (Inheritance)](#11-繼承-inheritance)
  - [1.2 多型 (Polymorphism)](#12-多型-polymorphism)
  - [1.3 封裝 (Encapsulation)](#13-封裝-encapsulation)
  - [1.4 抽象 (Abstraction)](#14-抽象-abstraction)
  - [1.5 組合 (Composition)](#15-組合-composition)
- [二、設計模式應用](#二設計模式應用)
  - [2.1 策略模式 (Strategy Pattern)](#21-策略模式-strategy-pattern)
  - [2.2 觀察者模式 (Observer Pattern)](#22-觀察者模式-observer-pattern)
  - [2.3 工廠模式 (Factory Pattern)](#23-工廠模式-factory-pattern)
  - [2.4 單例模式 (Singleton Pattern)](#24-單例模式-singleton-pattern)
  - [2.5 組件模式 (Component Pattern)](#25-組件模式-component-pattern)
  - [2.6 物件池模式 (Object Pool Pattern)](#26-物件池模式-object-pool-pattern)
  - [2.7 原型模式 (Prototype Pattern)](#27-原型模式-prototype-pattern)
  - [2.8 狀態模式 (State Pattern)](#28-狀態模式-state-pattern)
- [三、可改進之處](#三可改進之處)
  - [3.1 高優先級問題](#31-高優先級問題)
  - [3.2 中優先級問題](#32-中優先級問題)
  - [3.3 低優先級建議](#33-低優先級建議)
- [四、優秀實踐總結](#四優秀實踐總結)
- [五、統計數據](#五統計數據)

---

## 專案概述

**專案名稱：** 元氣騎士 (Soul Knight) C++ 複刻版
**專案類型：** 2D 隨機地牢射擊遊戲 (Roguelike Dungeon Shooter)
**程式語言：** C++ (C++17/20)
**建置系統：** CMake
**核心框架：** PTSD Framework
**架構風格：** 組件化架構 (Component-Based Architecture)，靈感來自 ECS (Entity-Component-System)

**核心功能：**
- 隨機生成地牢系統（5個小關卡，冰原主題）
- 戰鬥系統（武器、敵人、Boss：雪人王）
- 特殊房間（商店、寶箱）及 NPC
- 角色移動、射擊、閃避機制
- 道具與武器收集系統

---

## 一、已運用的 OOP 核心概念

### 1.1 繼承 (Inheritance)

#### **定義與用途**
繼承允許類別從基類繼承屬性和方法，建立「是一個」(is-a) 的關係。這促進了代碼複用並建立了類別層次結構。

#### **1.1.1 遊戲物件層次結構**

**位置：** [include/Override/nGameObject.hpp](include/Override/nGameObject.hpp#L14-L15)

```cpp
class nGameObject : public Util::GameObject,
                    public std::enable_shared_from_this<nGameObject>
{
    // 核心遊戲物件，所有遊戲實體的基類
    // 使用多重繼承：繼承框架的 GameObject 和 enable_shared_from_this
};
```

**繼承樹：**
```
Util::GameObject (PTSD 框架基類)
  └─> nGameObject (遊戲專用基類)
      ├─> Character (角色：玩家/敵人/NPC)
      ├─> Weapon (武器基類)
      │   ├─> MeleeWeapon (近戰武器)
      │   └─> GunWeapon (遠程武器)
      ├─> Attack (攻擊基類)
      │   ├─> Projectile (投射物)
      │   └─> EffectAttack (效果攻擊)
      ├─> Animation (動畫)
      ├─> DestructibleObject (可破壞物件)
      └─> WallObject (牆壁物件)
```

**為何使用：** 所有遊戲物件共享基本功能（位置、繪製、更新），避免重複代碼。

#### **1.1.2 武器層次結構**

**位置：** [include/Weapon/Weapon.hpp](include/Weapon/Weapon.hpp#L29)

```cpp
// 抽象基類
class Weapon : public nGameObject {
    virtual void attack(int damage, bool isCriticalHit = false) = 0;
    virtual std::shared_ptr<Weapon> Clone() const = 0;
protected:
    int m_damage;
    float m_attackInterval;
    AttackType m_AttackType;
};
```

**位置：** [include/Weapon/MeleeWeapon.hpp](include/Weapon/MeleeWeapon.hpp#L17)

```cpp
// 近戰武器實現
class MeleeWeapon final : public Weapon {
    void attack(int damage, bool isCriticalHit = false) override;
    std::shared_ptr<Weapon> Clone() const override;
};
```

**位置：** [include/Weapon/GunWeapon.hpp](include/Weapon/GunWeapon.hpp#L20)

```cpp
// 遠程武器實現（使用 final 防止進一步繼承）
class GunWeapon final : public Weapon {
    void attack(int damage, bool isCriticalHit = false) override;
    std::shared_ptr<Weapon> Clone() const override;
};
```

**為何使用：** 不同武器類型有共同屬性（傷害、攻擊間隔），但攻擊行為不同（近戰 vs 遠程）。

#### **1.1.3 房間層次結構**

**位置：** [include/Room/Room.hpp](include/Room/Room.hpp#L39)

```cpp
// 房間基類
class Room {
public:
    virtual void LoadFromJSON() = 0;
    virtual void Start(const std::shared_ptr<Character> &player);
    virtual void Update();
protected:
    std::vector<std::shared_ptr<Character>> m_Players;
    std::vector<std::shared_ptr<nGameObject>> m_RoomObjects;
};
```

**位置：** [include/Room/DungeonRoom.hpp](include/Room/DungeonRoom.hpp#L116)

```cpp
// 地牢房間中間類（添加網格系統、連接管理）
class DungeonRoom : public Room {
    virtual void TryActivateByPlayer() { SetState(RoomState::EXPLORED); }
    virtual void OnStateChanged() {}
private:
    std::unique_ptr<GridSystem> m_GridSystem;
    std::unique_ptr<RoomConnectionManager> m_ConnectionManager;
};
```

**具體房間類型（全部 final）：**
- [MonsterRoom](include/Room/MonsterRoom.hpp#L15) - 怪物房
- [BossRoom](include/Room/BossRoom.hpp#L17) - Boss 房
- [ChestRoom](include/Room/ChestRoom.hpp#L10) - 寶箱房
- [ShopRoom](include/Room/ShopRoom.hpp#L15) - 商店房
- [PortalRoom](include/Room/PortalRoom.hpp#L10) - 傳送門房
- [StartingRoom](include/Room/StartingRoom.hpp#L10) - 起始房

**為何使用：** 所有房間共享基本功能（物件管理、更新），但不同房間有特殊行為（戰鬥、購物）。

#### **1.1.4 場景層次結構**

**位置：** [include/Scene/Scene.hpp](include/Scene/Scene.hpp#L23-L49)

```cpp
class Scene {
public:
    // 純虛擬函數 - 強制子類實現
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
    virtual SceneType Change() = 0;

protected:
    std::shared_ptr<Room> m_CurrentRoom;
    std::shared_ptr<Util::Renderer> m_Root;
};
```

**場景實現：**
- [MainMenuScene](include/Scene/MainMenu_Scene.hpp#L17) - 主選單
- [LobbyScene](include/Scene/Lobby_Scene.hpp#L18) - 大廳
- [DungeonScene](include/Scene/Dungeon_Scene.hpp#L17) - 地牢（final）
- [ResultScene](include/Scene/Result_Scene.hpp#L16) - 結算
- [CompleteScene](include/Scene/Complete_Scene.hpp#L10) - 完成

**為何使用：** 所有場景有生命週期（啟動、更新、退出），但具體邏輯不同。

#### **1.1.5 組件層次結構**

**位置：** [include/Components/Component.hpp](include/Components/Component.hpp#L16-L29)

```cpp
class Component {
public:
    virtual void Init() {}
    virtual void Update() {}
    virtual void HandleCollision(const CollisionEventInfo &info) {}
    virtual void HandleEvent(const EventInfo& eventInfo) {}
    virtual std::vector<EventType> SubscribedEventTypes() const {return {};}
};
```

**20+ 組件實現：**
- HealthComponent - 生命值管理
- AttackComponent - 攻擊管理
- StateComponent - 狀態管理
- AIComponent - AI 行為（同時實現 TrackingObserver）
- AnimationComponent - 動畫控制
- CollisionComponent - 碰撞檢測
- InputComponent - 輸入處理
- MovementComponent - 移動控制
- SkillComponent - 技能系統
- ... 等等

**為何使用：** 組件化架構允許靈活組合功能，避免深層繼承。

---

### 1.2 多型 (Polymorphism)

#### **定義與用途**
多型允許通過基類指標/引用調用派生類的方法，實現「一個介面，多種實現」。使用 `virtual` 和 `override` 關鍵字。

#### **1.2.1 純虛擬函數（介面定義）**

**位置：** [include/Components/EnemyAI/AttackStrategy.hpp](include/Components/EnemyAI/AttackStrategy.hpp#L16-L27)

```cpp
// 攻擊策略介面
class IAttackStrategy {
public:
    virtual ~IAttackStrategy() = default;
    virtual void Update(const EnemyContext &ctx, const float deltaTime) = 0;
    [[nodiscard]] virtual bool CanAttack(const EnemyContext &ctx) = 0;
    [[nodiscard]] virtual float GetAttackDistance() const = 0;
};
```

**5 種具體實現：**

1. **近戰攻擊** [MeleeAttack](include/Components/EnemyAI/AttackStrategy.hpp#L32-L41)
```cpp
class MeleeAttack final : public IAttackStrategy {
    [[nodiscard]] float GetAttackDistance() const override {
        return m_meleeAttackDistance; // 50.0f
    }
};
```

2. **遠程攻擊** [GunAttack](include/Components/EnemyAI/AttackStrategy.hpp#L43-L52)
```cpp
class GunAttack final : public IAttackStrategy {
    [[nodiscard]] float GetAttackDistance() const override {
        return m_gunAttackDistance; // 200.0f
    }
};
```

3. **碰撞攻擊** [CollisionAttack](include/Components/EnemyAI/AttackStrategy.hpp#L54-L59)
```cpp
class CollisionAttack final : public IAttackStrategy {
    [[nodiscard]] bool CanAttack(const EnemyContext &ctx) override;
    [[nodiscard]] float GetAttackDistance() const override { return 0; }
};
```

4. **無攻擊** [NoAttack](include/Components/EnemyAI/AttackStrategy.hpp#L61-L66)
```cpp
class NoAttack final : public IAttackStrategy {
    void Update(const EnemyContext &ctx, const float deltaTime) override {}
    [[nodiscard]] bool CanAttack(const EnemyContext &ctx) override { return false; }
};
```

5. **Boss 攻擊策略** [BossAttackStrategy](include/Components/EnemyAI/AttackStrategy.hpp#L71-L127)
```cpp
class BossAttackStrategy : public IAttackStrategy {
    enum class BossSkillType {
        SKILL1,  // 雪球投擲
        SKILL2,  // 雪花風暴
        SKILL3,  // 火眼
        SKILL4,  // 冰霜荊棘
        SKILL5   // 雪人召喚
    };
    // 複雜的多技能系統
private:
    void ExecuteSkill1(const EnemyContext& ctx);  // 5 種技能分別實現
    void ExecuteSkill2(const EnemyContext& ctx);
    // ...
};
```

**執行時多型範例：**
```cpp
// AIComponent 中動態選擇攻擊策略
std::unordered_map<AttackStrategies, std::shared_ptr<IAttackStrategy>> m_attackStrategy;

// 執行時根據敵人類型調用不同實現
m_attackStrategy[currentStrategy]->Update(ctx, deltaTime);
// Boss 會執行 BossAttackStrategy::Update()
// 普通敵人會執行 MeleeAttack::Update() 或 GunAttack::Update()
```

**為何使用：** 不同敵人需要完全不同的攻擊邏輯（近戰、遠程、Boss 多技能），但 AI 組件統一調用。

#### **1.2.2 移動策略多型**

**位置：** [include/Components/EnemyAI/MoveStrategy.hpp](include/Components/EnemyAI/MoveStrategy.hpp#L22-L26)

```cpp
class IMoveStrategy {
public:
    virtual ~IMoveStrategy() = default;
    virtual void Update(const EnemyContext &ctx, float deltaTime) = 0;
};
```

**4 種移動策略：**

1. **徘徊移動** [WanderMove](include/Components/EnemyAI/MoveStrategy.hpp#L49-L53)
```cpp
class WanderMove final : public IMoveStrategy {
    void Update(const EnemyContext &ctx, const float deltaTime) override;
    // 隨機漫遊
};
```

2. **追逐移動** [ChaseMove](include/Components/EnemyAI/MoveStrategy.hpp#L55-L65)
```cpp
class ChaseMove final : public IMoveStrategy {
    void Update(const EnemyContext &ctx, float deltaTime) override;
    // 追蹤玩家，根據武器類型保持最佳距離
};
```

3. **靜止不動** [NoMove](include/Components/EnemyAI/MoveStrategy.hpp#L67-L72)
```cpp
class NoMove final : public IMoveStrategy {
    void Update(const EnemyContext &ctx, float deltaTime) override {}
};
```

4. **Boss 移動** [BossMove](include/Components/EnemyAI/MoveStrategy.hpp#L74-L95)
```cpp
class BossMove final : public IMoveStrategy {
    void Update(const EnemyContext& ctx, float deltaTime) override;
    // 複雜的跳躍、技能位移邏輯
private:
    void UpdateSkill1MoveState(const EnemyContext& ctx, float deltaTime);
    void UpdateSkill2MoveState(const EnemyContext& ctx, float deltaTime);
    // 每個技能有不同的移動模式
};
```

**為何使用：** 敵人移動行為差異巨大（徘徊 vs 追逐 vs Boss 複雜移動），策略模式提供靈活切換。

#### **1.2.3 觸發器策略多型**

**位置：** [include/TriggerStrategy/ITriggerStrategy.hpp](include/TriggerStrategy/ITriggerStrategy.hpp#L12-L18)

```cpp
class ITriggerStrategy {
public:
    virtual ~ITriggerStrategy() = default;
    virtual void OnTriggerEnter(std::shared_ptr<nGameObject> self,
                                std::shared_ptr<nGameObject> other) = 0;
    virtual void OnTriggerStay(...) {};   // 可選
    virtual void OnTriggerExit(...) {};  // 可選
};
```

**具體策略：**
- [BlockProjectileStrategy](include/TriggerStrategy/BlockProjectileStrategy.hpp#L12) - 阻擋投射物
- [KnockOffTriggerStrategy](include/TriggerStrategy/KnockOffTriggerStrategy.hpp#L10) - 擊退效果
- [ReflectTriggerStrategy](include/TriggerStrategy/ReflectTriggerStrategy.hpp#L10) - 反彈投射物
- [SpeedZoneStrategy](include/TriggerStrategy/SpeedZoneStrategy.hpp#L10) - 速度區域
- [AttackTriggerStrategy](include/TriggerStrategy/AttackTriggerStrategy.hpp#L11) - 觸發傷害

**為何使用：** 不同觸發器有完全不同的效果（阻擋、擊退、反彈），統一介面方便管理。

#### **1.2.4 模板多型**

**位置：** [include/Override/nGameObject.hpp](include/Override/nGameObject.hpp#L27-L30)

```cpp
template <typename T, typename... Args>
std::shared_ptr<T> AddComponent(ComponentType type, Args &&...args);

template <typename T>
std::shared_ptr<T> GetComponent(ComponentType type);
```

**使用範例：**
```cpp
auto healthComp = gameObject->GetComponent<HealthComponent>(ComponentType::HEALTH);
auto aiComp = gameObject->GetComponent<AIComponent>(ComponentType::AI);
```

**為何使用：** 型別安全的組件管理，編譯時確定類型。

---

### 1.3 封裝 (Encapsulation)

#### **定義與用途**
封裝隱藏內部實現細節，只暴露必要的介面。使用 `private`、`protected`、`public` 訪問修飾符，配合 getter/setter。

#### **1.3.1 優秀封裝範例 - HealthComponent**

**位置：** [include/Components/HealthComponent.hpp](include/Components/HealthComponent.hpp#L76-L88)

```cpp
class HealthComponent : public Component {
public:
    // Getters - 使用 [[nodiscard]] 確保返回值被使用
    [[nodiscard]] int GetMaxHp() const { return m_maxHp; }
    [[nodiscard]] int GetCurrentHp() const { return m_currentHp; }
    [[nodiscard]] int GetMaxArmor() const { return m_maxArmor; }
    [[nodiscard]] int GetCurrentArmor() const { return m_currentArmor; }

    // Setters - 帶驗證邏輯
    void SetCurrentHp(const int hp) {
        if(hp <= m_maxHp) m_currentHp = hp;
        if (m_currentHp <= 0) OnDeath();  // 自動觸發死亡
    }

    void AddCurrentHp(const int hp) {
        m_currentHp += hp;
        if (m_currentHp > m_maxHp) m_currentHp = m_maxHp;  // 不超過最大值
    }

protected:
    void OnDeath();  // 內部邏輯，外部不可見
    void OnBreak();

private:
    int m_maxHp;
    int m_currentHp;
    int m_maxArmor;
    int m_currentArmor;
    int m_maxEnergy;
    int m_currentEnergy;
    // 所有數據私有，只能通過方法訪問
};
```

**優點：**
- ✅ 所有成員變數 `private`
- ✅ Setter 包含驗證（不超過最大值、自動觸發死亡）
- ✅ 使用 `[[nodiscard]]` 防止忽略返回值
- ✅ 保護的輔助方法（`OnDeath`、`OnBreak`）

#### **1.3.2 優秀封裝範例 - Weapon**

**位置：** [include/Weapon/Weapon.hpp](include/Weapon/Weapon.hpp#L36-L63)

```cpp
class Weapon : public nGameObject {
public:
    // 只讀 Getters - 全部使用 [[nodiscard]] const
    [[nodiscard]] int GetWeaponID() const { return m_ID; }
    [[nodiscard]] AttackType GetAttackType() const { return m_AttackType; }
    [[nodiscard]] int GetDamage() const { return m_damage; }
    [[nodiscard]] float GetCriticalRate() const { return m_criticalRate; }

    // Setters
    void SetOffset(const int offset) { m_offset = offset; }
    void SetAttackDelay(float delay) {
        m_attackDelay = delay;
        lastAttackTime = m_attackDelay;  // 同時更新計時器
    }

    // 公共介面
    int calculateDamage();
    bool CanAttack();
    virtual void attack(int damage, bool isCriticalHit = false) = 0;

protected:
    // 子類可訪問但外部不可見
    int m_ID;
    int m_damage;
    float m_criticalRate;
    float m_attackInterval;
    int m_offset;
    AttackType m_AttackType;
    WeaponType m_weaponType;
    std::shared_ptr<Character> m_currentOwner;
};
```

**優點：**
- ✅ Protected 成員允許繼承但隱藏外部
- ✅ Setter 有副作用邏輯（更新計時器）
- ✅ 純虛擬方法強制子類實現

#### **1.3.3 優秀封裝範例 - Component**

**位置：** [include/Components/Component.hpp](include/Components/Component.hpp#L31-L44)

```cpp
class Component {
public:
    void SetOwner(const std::shared_ptr<nGameObject> &owner) {
        m_Owner = owner;  // 內部管理
    }

    template <typename T>
    std::shared_ptr<T> GetOwner() const {
        // 型別安全的訪問，使用 weak_ptr 防止循環引用
        return std::dynamic_pointer_cast<T>(m_Owner.lock());
    }

    [[nodiscard]] ComponentType GetType() const { return m_type; }

private:
    std::weak_ptr<nGameObject> m_Owner;  // 使用 weak_ptr 打破循環引用
    ComponentType m_type;
};
```

**優點：**
- ✅ 使用 `weak_ptr` 防止循環引用（Component 持有 GameObject，GameObject 持有 Component）
- ✅ 模板 getter 提供型別安全訪問
- ✅ 私有成員只能通過方法訪問

#### **1.3.4 刪除的複製/移動構造 - Character**

**位置：** [include/Creature/Character.hpp](include/Creature/Character.hpp#L18-L21)

```cpp
class Character final : public nGameObject {
public:
    // 刪除複製和移動操作 - 確保唯一性
    Character(const Character &) = delete;
    Character(Character &&) = delete;
    Character &operator=(const Character &) = delete;
    Character &operator=(Character &&) = delete;
};
```

**為何使用：** 角色物件應該唯一，防止意外複製導致狀態不一致。

#### **1.3.5 封裝違規範例（需改進）**

**位置：** [include/Override/nGameObject.hpp](include/Override/nGameObject.hpp#L24)

```cpp
class nGameObject : public Util::GameObject {
public:
    glm::vec2 m_WorldCoord = {0, 0};  // ❌ 公有數據成員！

    // 明明已經有正確的 getter/setter
    void SetWorldCoord(const glm::vec2 coord) {
        m_WorldCoord = coord + m_PosOffset;  // 有偏移補償邏輯
    }
    [[nodiscard]] glm::vec2 GetWorldCoord() const { return m_WorldCoord; }

protected:
    glm::vec2 m_PosOffset = glm::vec2(0.0f);
};
```

**問題：**
- ❌ `m_WorldCoord` 是公有的，外部可以直接修改
- ❌ 繞過 `SetWorldCoord` 中的偏移補償邏輯
- ❌ 可能導致 Bug：`obj.m_WorldCoord = coord;` vs `obj.SetWorldCoord(coord);`

**建議修復：**
```cpp
class nGameObject : public Util::GameObject {
public:
    void SetWorldCoord(const glm::vec2 coord) {
        m_WorldCoord = coord + m_PosOffset;
    }
    [[nodiscard]] glm::vec2 GetWorldCoord() const { return m_WorldCoord; }

protected:  // 或 private
    glm::vec2 m_WorldCoord = {0, 0};  // ✅ 移到 protected
    glm::vec2 m_PosOffset = glm::vec2(0.0f);
};
```

---

### 1.4 抽象 (Abstraction)

#### **定義與用途**
抽象通過抽象類和介面定義高層次的契約，隱藏實現細節，只暴露必要功能。使用純虛擬函數 (`= 0`) 定義介面。

#### **1.4.1 管理器介面 - IManager**

**位置：** [include/ObserveManager/IManager.hpp](include/ObserveManager/IManager.hpp#L8-L10)

```cpp
class IManager {
public:
    virtual void Update() = 0;  // 純虛擬 - 強制實現
    virtual ~IManager() = default;
};
```

**具體實現：**
- `InputManager` - 輸入管理
- `TrackingManager` - 追蹤管理
- `AttackManager` - 攻擊管理
- `RoomCollisionManager` - 碰撞管理

**為何使用：** 所有管理器需要在遊戲循環中更新，統一介面方便管理。

#### **1.4.2 場景抽象類 - Scene**

**位置：** [include/Scene/Scene.hpp](include/Scene/Scene.hpp#L23-L49)

```cpp
class Scene {
public:
    // 純虛擬 - 定義場景生命週期
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
    virtual SceneType Change() = 0;

    // 可選的虛擬函數（有默認實現）
    virtual void Upload();
    virtual void Download();

    // 非虛擬公共介面
    std::shared_ptr<Room> GetCurrentRoom() { return m_CurrentRoom; }
    bool IsChange() const { return m_IsChange; }

protected:
    bool m_IsChange = false;
    SceneType m_SceneType;
    std::shared_ptr<Room> m_CurrentRoom;
};
```

**實現範例：** [DungeonScene](include/Scene/Dungeon_Scene.hpp#L17)
```cpp
class DungeonScene final : public Scene {
    void Start() override;    // 初始化地牢
    void Update() override;   // 更新遊戲邏輯
    void Exit() override;     // 清理資源
    SceneType Change() override;  // 切換場景邏輯
};
```

**為何使用：** 強制所有場景實現生命週期方法，確保一致性。

#### **1.4.3 區域形狀抽象 - AreaShape**

**位置：** [include/Structs/AreaShape.hpp](include/Structs/AreaShape.hpp#L37-L74)

```cpp
class AreaShape {
public:
    virtual ~AreaShape() = default;

    // 純虛擬 - 型別識別
    virtual ShapeType GetType() const = 0;
    virtual void SetCenter(const glm::vec2& center) = 0;

    // 碰撞檢測介面
    virtual bool Intersects(const std::shared_ptr<AreaShape>& other) const = 0;
    virtual bool IntersectsWithRect(const RectShape& other) const = 0;
    virtual bool IntersectsWithCircle(const CircleShape& other) const = 0;
    virtual bool ContainsPoint(const glm::vec2& other) const = 0;
};

// 具體實現
class RectShape : public AreaShape { /* ... */ };
class CircleShape : public AreaShape { /* ... */ };
```

**為何使用：** 矩形和圓形碰撞檢測算法完全不同，但需要統一介面。

#### **1.4.4 破壞效果抽象 - IDestructionEffect**

**位置：** [include/DestructionEffects/IDestructionEffect.hpp](include/DestructionEffects/IDestructionEffect.hpp#L16-L33)

```cpp
class IDestructionEffect {
public:
    virtual ~IDestructionEffect() = default;
    virtual void Execute(const glm::vec2 &position, CharacterType attackerType) = 0;
    virtual std::string GetEffectName() const = 0;
};
```

**具體效果：**
- [ExplosionEffect](include/DestructionEffects/ExplosionEffect.hpp#L14) - 爆炸
- [IceSpikeEffect](include/DestructionEffects/IceSpikeEffect.hpp#L14) - 冰刺
- [PoisonCloudEffect](include/DestructionEffects/PoisonCloudEffect.hpp#L15) - 毒雲

**為何使用：** 不同破壞效果（爆炸、冰刺、毒雲）有完全不同的視覺和傷害邏輯。

---

### 1.5 組合 (Composition)

#### **定義與用途**
組合是「有一個」(has-a) 的關係，通過包含其他類的實例來構建功能。相比繼承更靈活（「組合優於繼承」原則）。

#### **1.5.1 組件容器 - nGameObject**

**位置：** [include/Override/nGameObject.hpp](include/Override/nGameObject.hpp#L27-L39)

```cpp
class nGameObject : public Util::GameObject {
public:
    // 添加組件（使用可變參數模板）
    template <typename T, typename... Args>
    std::shared_ptr<T> AddComponent(ComponentType type, Args &&...args);

    // 獲取組件
    template <typename T>
    std::shared_ptr<T> GetComponent(ComponentType type);

    // 移除組件
    template <typename T>
    bool RemoveComponent(ComponentType type);

private:
    // 組件容器
    std::unordered_map<ComponentType, std::shared_ptr<Component>> m_Components;
};
```

**使用範例：**
```cpp
// 玩家角色組合
auto player = std::make_shared<Character>();
player->AddComponent<HealthComponent>(ComponentType::HEALTH, 100, 50);
player->AddComponent<InputComponent>(ComponentType::INPUT);
player->AddComponent<AttackComponent>(ComponentType::ATTACK);
player->AddComponent<MovementComponent>(ComponentType::MOVEMENT);

// 敵人組合
auto enemy = std::make_shared<Character>();
enemy->AddComponent<HealthComponent>(ComponentType::HEALTH, 50, 0);
enemy->AddComponent<AIComponent>(ComponentType::AI);  // 不是 InputComponent
enemy->AddComponent<AttackComponent>(ComponentType::ATTACK);
```

**為何使用：**
- ✅ 靈活組合功能（玩家有輸入組件，敵人有 AI 組件）
- ✅ 避免深層繼承（不需要 PlayerCharacter、EnemyCharacter 子類）
- ✅ 執行時添加/移除功能

#### **1.5.2 策略組合 - AIComponent**

**位置：** [include/Components/AiComponent.hpp](include/Components/AiComponent.hpp#L68-L78)

```cpp
class AIComponent : public Component, public TrackingObserver {
private:
    // 組合不同的策略物件
    std::shared_ptr<IMoveStrategy> m_moveStrategy;
    std::unordered_map<AttackStrategies, std::shared_ptr<IAttackStrategy>> m_attackStrategy;
    std::shared_ptr<IUtilityStrategy> m_utilityStrategy;

    // 組合其他組件引用
    std::shared_ptr<AttackComponent> m_AttackComponent;
    std::weak_ptr<nGameObject> m_Target;
    std::shared_ptr<nGameObject> m_readyAttackIcon;
};
```

**為何使用：** AI 行為由多個獨立策略組成，可獨立替換（例如改變移動策略不影響攻擊策略）。

#### **1.5.3 管理器組合 - Room**

**位置：** [include/Room/Room.hpp](include/Room/Room.hpp#L132)

```cpp
class Room {
protected:
    // 組合多個管理器
    std::unordered_map<ManagerTypes, std::shared_ptr<IManager>> m_Managers;
    std::shared_ptr<RoomCollisionManager> m_CollisionManager;
    std::shared_ptr<RoomInteractionManager> m_InteractionManager;
    std::shared_ptr<TrackingManager> m_TrackingManager;
};
```

**為何使用：** 房間「有」碰撞管理器、交互管理器，而不是「是」一個管理器。

#### **1.5.4 Unique Ptr 組合 - DungeonRoom**

**位置：** [include/Room/DungeonRoom.hpp](include/Room/DungeonRoom.hpp#L191-L193)

```cpp
class DungeonRoom : public Room {
private:
    // 使用 unique_ptr 確保唯一擁有權
    std::unique_ptr<GridSystem> m_GridSystem;
    std::unique_ptr<RoomConnectionManager> m_ConnectionManager;
    std::unique_ptr<TerrainGenerator> m_TerrainGenerator;
};
```

**為何使用：**
- ✅ `unique_ptr` 確保只有一個 DungeonRoom 擁有這些系統
- ✅ 自動內存管理（房間銷毀時自動清理）
- ✅ 明確所有權語義

#### **1.5.5 物件池組合 - AttackManager**

**位置：** [include/Attack/AttackManager.hpp](include/Attack/AttackManager.hpp#L40-L47)

```cpp
class AttackManager : public IManager {
private:
    // 活躍物件列表
    std::vector<std::shared_ptr<Projectile>> m_projectiles;
    std::vector<std::shared_ptr<EffectAttack>> m_effects;

    // 組合物件池
    ProjectilePool m_projectilePool;
    EffectAttackPool m_effectPool;
};
```

**為何使用：** 管理器「有」物件池來重用投射物，減少記憶體分配。

---

## 二、設計模式應用

### 2.1 策略模式 (Strategy Pattern)

#### **定義與用途**
策略模式定義一系列算法，將每個算法封裝起來，使它們可以互相替換。策略模式讓算法的變化獨立於使用算法的客戶端。

#### **2.1.1 攻擊策略**

**位置：** [include/Components/EnemyAI/AttackStrategy.hpp](include/Components/EnemyAI/AttackStrategy.hpp)

**結構：**
```cpp
// 策略介面
class IAttackStrategy {
    virtual void Update(const EnemyContext &ctx, const float deltaTime) = 0;
    virtual bool CanAttack(const EnemyContext &ctx) = 0;
    virtual float GetAttackDistance() const = 0;
};

// 具體策略
class MeleeAttack final : public IAttackStrategy { /* 近戰邏輯 */ };
class GunAttack final : public IAttackStrategy { /* 遠程邏輯 */ };
class BossAttackStrategy : public IAttackStrategy { /* 複雜多技能邏輯 */ };
class NoAttack final : public IAttackStrategy { /* 不攻擊 */ };
```

**使用場景：** AIComponent 根據敵人類型選擇策略
```cpp
// 在 AIComponent 中
std::unordered_map<AttackStrategies, std::shared_ptr<IAttackStrategy>> m_attackStrategy;

// 執行時切換
if (isBoss) {
    currentStrategy = AttackStrategies::BOSS;
} else if (hasGun) {
    currentStrategy = AttackStrategies::GUN;
} else {
    currentStrategy = AttackStrategies::MELEE;
}

m_attackStrategy[currentStrategy]->Update(ctx, deltaTime);
```

**優點：**
- ✅ 新增攻擊類型無需修改 AIComponent
- ✅ 每種攻擊策略獨立測試
- ✅ Boss 可以有完全不同的攻擊邏輯

#### **2.1.2 移動策略**

**位置：** [include/Components/EnemyAI/MoveStrategy.hpp](include/Components/EnemyAI/MoveStrategy.hpp#L22-L95)

**策略家族：**
- `WanderMove` - 隨機徘徊（哥布林）
- `ChaseMove` - 追逐玩家（大部分敵人）
- `NoMove` - 固定位置（砲台）
- `BossMove` - 複雜移動（Boss 跳躍、技能位移）

**使用範例：**
```cpp
// Boss 使用特殊移動
auto bossAI = boss->GetComponent<AIComponent>(ComponentType::AI);
bossAI->SetMoveStrategy(std::make_shared<BossMove>());

// 普通敵人使用追逐
auto enemyAI = enemy->GetComponent<AIComponent>(ComponentType::AI);
enemyAI->SetMoveStrategy(std::make_shared<ChaseMove>());
```

**優點：**
- ✅ 移動邏輯與 AI 組件解耦
- ✅ 可執行時切換策略（例如 Boss 階段變化）

#### **2.1.3 觸發器策略**

**位置：** [include/TriggerStrategy/](include/TriggerStrategy/)

**策略列表：**
- `BlockProjectileStrategy` - 阻擋投射物（牆壁）
- `ReflectTriggerStrategy` - 反彈投射物（護盾）
- `KnockOffTriggerStrategy` - 擊退效果（爆炸波）
- `SpeedZoneStrategy` - 速度區域（冰面/加速帶）
- `AttackTriggerStrategy` - 觸發傷害（毒霧）

**為何使用：** 不同遊戲物件有完全不同的碰撞效果，策略模式避免巨大的 switch-case。

---

### 2.2 觀察者模式 (Observer Pattern)

#### **定義與用途**
觀察者模式定義物件間一對多的依賴關係，當一個物件狀態改變時，所有依賴者都會收到通知並自動更新。

#### **2.2.1 輸入觀察者**

**位置：** [include/Observer.hpp](include/Observer.hpp#L14-L18)

```cpp
// 觀察者介面
class InputObserver {
public:
    virtual void onInputReceived(const std::set<char>& keys) = 0;
    virtual ~InputObserver() = default;
};
```

**主題（Subject）：** [include/ObserveManager/InputManager.hpp](include/ObserveManager/InputManager.hpp)
```cpp
class InputManager : public ObserveManager {
public:
    void notifyObserver() override {
        for (auto& observer : m_Observer) {
            observer->onInputReceived(m_currentKeys);
        }
    }

    void addObserver(const std::shared_ptr<InputObserver> &observer);
    void removeObserver(const std::shared_ptr<InputObserver> &observer);
};
```

**觀察者實現：** InputComponent
```cpp
class InputComponent : public Component, public InputObserver {
    void onInputReceived(const std::set<char>& keys) override {
        // 處理鍵盤輸入，更新角色狀態
    }
};
```

**優點：**
- ✅ 輸入管理器不需要知道誰在監聽
- ✅ 動態添加/移除觀察者（角色死亡時移除）
- ✅ 解耦輸入處理和遊戲邏輯

#### **2.2.2 追蹤觀察者**

**位置：** [include/Observer.hpp](include/Observer.hpp#L21-L31)

```cpp
class TrackingObserver {
public:
    virtual ~TrackingObserver() = default;
    virtual void OnTargetPositionUpdate(std::weak_ptr<Character> enemy) {}
    virtual void OnLostTarget() {}
    virtual void OnPlayerPositionUpdate(std::weak_ptr<Character> player) {}
    virtual void OnLostPlayer() {}
};
```

**使用場景：** AIComponent 和 AttackComponent 監聽玩家/目標位置
```cpp
class AIComponent : public Component, public TrackingObserver {
    void OnPlayerPositionUpdate(std::weak_ptr<Character> player) override {
        // 更新追蹤目標位置
        m_moveStrategy->Update(ctx, deltaTime);
    }

    void OnLostPlayer() override {
        // 玩家離開範圍，切換到徘徊狀態
        SetState(enemyState::WANDERING);
    }
};
```

**優點：**
- ✅ 敵人自動追蹤玩家位置變化
- ✅ 減少輪詢（不需要每幀計算距離）

#### **2.2.3 事件管理器**

**位置：** [include/ObserveManager/EventManager.hpp](include/ObserveManager/EventManager.hpp#L19-L76)

```cpp
class EventManager {
public:
    static EventManager &GetInstance() {
        static EventManager instance;
        return instance;
    }

    // 類型安全的事件系統
    template <typename EventT>
    void AddListener(const std::string &eventName,
                     std::function<void(const EventT &)> callback);

    template <typename EventT>
    void TriggerEvent(const std::string &eventName, const EventT &eventData);
};
```

**使用範例：**
```cpp
// 監聽敵人死亡事件
EventManager::GetInstance().AddListener<EnemyDeathEvent>(
    "EnemyDeath",
    [](const EnemyDeathEvent& event) {
        // 掉落物品、增加分數等
    }
);

// 觸發事件
EventManager::GetInstance().TriggerEvent("EnemyDeath", EnemyDeathEvent{enemyId, position});
```

---

### 2.3 工廠模式 (Factory Pattern)

#### **定義與用途**
工廠模式定義創建物件的介面，但讓子類決定實例化哪個類。工廠方法將物件的實例化推遲到子類。

#### **2.3.1 角色工廠（結合單例）**

**位置：** [include/Factory/CharacterFactory.hpp](include/Factory/CharacterFactory.hpp#L13-L42)

```cpp
class CharacterFactory final : public Factory {
public:
    // 單例模式
    static CharacterFactory& GetInstance() {
        static CharacterFactory instance;
        return instance;
    }

    // 工廠方法
    std::shared_ptr<Character> createPlayer(const int id);
    std::shared_ptr<Character> createEnemy(const int id);
    std::shared_ptr<Character> CloneEnemy(const std::shared_ptr<Character>& original);
    std::shared_ptr<Character> createNPC(const int id);

private:
    // 私有構造 - 單例模式
    CharacterFactory() {
        enemyJsonData = readJsonFile("enemy.json");
        npcJsonData = readJsonFile("npc.json");
    }

    // 禁止複製
    CharacterFactory(const CharacterFactory&) = delete;
    void operator=(const CharacterFactory&) = delete;

    // 緩存 JSON 數據
    nlohmann::json enemyJsonData;
    nlohmann::json npcJsonData;
};
```

**使用範例：**
```cpp
// 創建玩家
auto player = CharacterFactory::GetInstance().createPlayer(1);

// 創建敵人
auto goblin = CharacterFactory::GetInstance().createEnemy(101);
auto orc = CharacterFactory::GetInstance().createEnemy(102);

// 複製敵人（Boss 召喚小怪）
auto clone = CharacterFactory::GetInstance().CloneEnemy(goblin);
```

**優點：**
- ✅ 所有角色創建邏輯集中管理
- ✅ 從 JSON 讀取配置，數據驅動
- ✅ 單例確保只讀取一次 JSON
- ✅ 緩存提升性能

#### **2.3.2 武器工廠**

**位置：** [include/Factory/WeaponFactory.hpp](include/Factory/WeaponFactory.hpp#L10-L14)

```cpp
namespace WeaponFactory {
    std::shared_ptr<Weapon> createWeapon(const int id);
    std::shared_ptr<Weapon> createWeaponByName(const std::string &name);
}
```

**為何使用：** 根據 ID 或名稱創建不同類型的武器（近戰/遠程），自動配置屬性。

#### **2.3.3 房間工廠**

**位置：** [include/Factory/RoomFactory.hpp](include/Factory/RoomFactory.hpp)

**創建不同類型房間：**
```cpp
std::shared_ptr<Room> RoomFactory::createRoom(RoomType type) {
    switch (type) {
        case RoomType::MONSTER:
            return std::make_shared<MonsterRoom>();
        case RoomType::BOSS:
            return std::make_shared<BossRoom>();
        case RoomType::SHOP:
            return std::make_shared<ShopRoom>();
        case RoomType::CHEST:
            return std::make_shared<ChestRoom>();
        // ...
    }
}
```

**優點：**
- ✅ 客戶端不需要知道具體房間類
- ✅ 集中控制房間創建邏輯

---

### 2.4 單例模式 (Singleton Pattern)

#### **定義與用途**
單例模式確保一個類只有一個實例，並提供全域訪問點。適用於全域唯一的管理器。

#### **2.4.1 圖片池管理器**

**位置：** [include/ImagePoolManager.hpp](include/ImagePoolManager.hpp#L18-L51)

```cpp
class ImagePoolManager {
public:
    // 全域訪問點
    static ImagePoolManager& GetInstance() {
        static ImagePoolManager instance;  // C++11 線程安全
        return instance;
    }

    std::shared_ptr<Util::Image> GetImage(const std::string& path);
    void ClearCache();

private:
    // 私有構造
    ImagePoolManager() = default;
    ~ImagePoolManager() = default;

    // 禁止複製和賦值
    ImagePoolManager(const ImagePoolManager&) = delete;
    ImagePoolManager& operator=(const ImagePoolManager&) = delete;

    // 圖片緩存
    std::unordered_map<std::string, std::shared_ptr<Util::Image>> m_imagePool;
};
```

**使用範例：**
```cpp
// 全域唯一的圖片池
auto image = ImagePoolManager::GetInstance().GetImage("player.png");
auto sameImage = ImagePoolManager::GetInstance().GetImage("player.png");  // 返回緩存
```

**優點：**
- ✅ 確保只有一個圖片池
- ✅ 避免重複載入相同圖片
- ✅ C++11 靜態局部變數保證線程安全

#### **2.4.2 場景管理器**

**位置：** [include/Scene/SceneManager.hpp](include/Scene/SceneManager.hpp#L11-L58)

```cpp
class SceneManager {
public:
    static SceneManager &GetInstance() {
        static SceneManager instance;
        return instance;
    }

    void SetNextScene(Scene::SceneType sceneType);
    void ChangeToNextScene();
    std::shared_ptr<Scene> GetCurrentScene() { return m_CurrentScene; }

    // 禁止複製
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

private:
    SceneManager() = default;
    std::shared_ptr<Scene> m_CurrentScene;
};
```

**為何使用：** 遊戲只能有一個當前場景，全域訪問方便場景切換。

#### **2.4.3 事件管理器**

**位置：** [include/ObserveManager/EventManager.hpp](include/ObserveManager/EventManager.hpp#L19-L27)

```cpp
class EventManager {
public:
    static EventManager &GetInstance() {
        static EventManager instance;
        return instance;
    }
    // 事件系統...
};
```

---

### 2.5 組件模式 (Component Pattern)

#### **定義與用途**
組件模式將物件的功能拆分成獨立的組件，通過組合而非繼承實現功能。靈感來自 ECS (Entity-Component-System)。

#### **2.5.1 組件基類**

**位置：** [include/Components/Component.hpp](include/Components/Component.hpp#L16-L46)

```cpp
class Component {
public:
    virtual void Init() {}
    virtual void Update() {}
    virtual void HandleCollision(const CollisionEventInfo &info) {}
    virtual void HandleEvent(const EventInfo& eventInfo) {}
    virtual std::vector<EventType> SubscribedEventTypes() const {return {};}

    void SetOwner(const std::shared_ptr<nGameObject> &owner) { m_Owner = owner; }
    template <typename T>
    std::shared_ptr<T> GetOwner() const {
        return std::dynamic_pointer_cast<T>(m_Owner.lock());
    }

private:
    std::weak_ptr<nGameObject> m_Owner;
    ComponentType m_type;
};
```

#### **2.5.2 20+ 組件類型**

**核心組件：**
1. **HealthComponent** - 生命值、護甲、能量管理
2. **AttackComponent** - 攻擊邏輯、武器管理、傷害計算
3. **StateComponent** - 狀態管理（移動/站立/攻擊/死亡）
4. **AIComponent** - 敵人 AI（策略組合）
5. **InputComponent** - 玩家輸入處理
6. **MovementComponent** - 移動控制、速度管理
7. **AnimationComponent** - 動畫播放、狀態同步
8. **CollisionComponent** - 碰撞檢測、碰撞盒管理
9. **SkillComponent** - 技能系統
10. **TalentComponent** - 天賦系統

**特殊組件：**
- **ProjectileComponent** - 投射物行為（追蹤、反彈）
- **FollowerComponent** - 跟隨邏輯（武器跟隨角色）
- **TriggerComponent** - 觸發器（組合觸發策略）
- **DestructibleComponent** - 可破壞物件（組合破壞效果）

#### **2.5.3 組件組合範例**

**玩家角色組合：**
```cpp
auto player = std::make_shared<Character>(CharacterType::PLAYER);
player->AddComponent<HealthComponent>(ComponentType::HEALTH, 100, 50, 200);
player->AddComponent<InputComponent>(ComponentType::INPUT);
player->AddComponent<AttackComponent>(ComponentType::ATTACK);
player->AddComponent<MovementComponent>(ComponentType::MOVEMENT);
player->AddComponent<StateComponent>(ComponentType::STATE);
player->AddComponent<AnimationComponent>(ComponentType::ANIMATION);
player->AddComponent<SkillComponent>(ComponentType::SKILL);
player->AddComponent<TalentComponent>(ComponentType::TALENT);
```

**近戰敵人組合：**
```cpp
auto goblin = std::make_shared<Character>(CharacterType::ENEMY);
goblin->AddComponent<HealthComponent>(ComponentType::HEALTH, 30, 0, 0);
goblin->AddComponent<AIComponent>(ComponentType::AI);  // AI 替代 Input
goblin->AddComponent<AttackComponent>(ComponentType::ATTACK);
goblin->AddComponent<MovementComponent>(ComponentType::MOVEMENT);
goblin->AddComponent<StateComponent>(ComponentType::STATE);
goblin->AddComponent<AnimationComponent>(ComponentType::ANIMATION);

// AI 組件配置策略
auto ai = goblin->GetComponent<AIComponent>(ComponentType::AI);
ai->SetMoveStrategy(std::make_shared<ChaseMove>());
ai->SetAttackStrategy(AttackStrategies::MELEE, std::make_shared<MeleeAttack>());
```

**Boss 組合：**
```cpp
auto boss = std::make_shared<Character>(CharacterType::BOSS);
boss->AddComponent<HealthComponent>(ComponentType::HEALTH, 1000, 100, 0);
boss->AddComponent<AIComponent>(ComponentType::AI);
boss->AddComponent<AttackComponent>(ComponentType::ATTACK);
boss->AddComponent<MovementComponent>(ComponentType::MOVEMENT);
// ... 其他組件

auto ai = boss->GetComponent<AIComponent>(ComponentType::AI);
ai->SetMoveStrategy(std::make_shared<BossMove>());  // 特殊移動
ai->SetAttackStrategy(AttackStrategies::BOSS, std::make_shared<BossAttackStrategy>());  // 多技能
```

**優點：**
- ✅ **靈活性：** 不同角色類型只是組件組合不同
- ✅ **可擴展：** 新功能只需新增組件
- ✅ **可重用：** 同一組件可用於不同實體
- ✅ **避免深層繼承：** 不需要 `PlayerCharacter`、`EnemyCharacter`、`BossCharacter` 子類

---

### 2.6 物件池模式 (Object Pool Pattern)

#### **定義與用途**
物件池模式通過重用物件而非頻繁創建/銷毀來提升性能。適用於頻繁創建的短生命週期物件。

#### **2.6.1 投射物池**

**位置：** [include/Attack/ProjectilePool.hpp](include/Attack/ProjectilePool.hpp#L11-L26)

```cpp
class ProjectilePool {
public:
    // 從池中獲取投射物
    std::shared_ptr<Projectile> Acquire(const ProjectileInfo& projectileInfo);

    // 歸還到池中
    void Release(const std::shared_ptr<Projectile>& bullet);

private:
    static std::deque<std::shared_ptr<Projectile>> pool;
};
```

**工作流程：**
```cpp
// AttackManager 使用物件池
class AttackManager : public IManager {
    void CreateProjectile(const ProjectileInfo& info) {
        // 從池中獲取（而非 new）
        auto bullet = m_projectilePool.Acquire(info);
        m_projectiles.push_back(bullet);
    }

    void Update() override {
        for (auto it = m_projectiles.begin(); it != m_projectiles.end();) {
            if ((*it)->ShouldDestroy()) {
                // 歸還到池中（而非 delete）
                m_projectilePool.Release(*it);
                it = m_projectiles.erase(it);
            } else {
                ++it;
            }
        }
    }

private:
    ProjectilePool m_projectilePool;
};
```

**優點：**
- ✅ 減少記憶體分配/釋放（射擊遊戲每秒可能創建數百個子彈）
- ✅ 減少 GC 壓力（如果使用垃圾回收）
- ✅ 提升性能（特別是高頻創建場景）

#### **2.6.2 效果攻擊池**

**位置：** [include/Attack/EffectAttackPool.hpp](include/Attack/EffectAttackPool.hpp#L11-L23)

```cpp
class EffectAttackPool {
public:
    std::shared_ptr<EffectAttack> Acquire(const EffectAttackInfo& effectInfo);
    void Release(const std::shared_ptr<EffectAttack>& effect);
private:
    static std::deque<std::shared_ptr<EffectAttack>> pool;
};
```

**為何使用：** 近戰武器頻繁創建效果攻擊（劍氣、斧頭揮砍），池化避免性能瓶頸。

---

### 2.7 原型模式 (Prototype Pattern)

#### **定義與用途**
原型模式通過複製現有物件來創建新物件，避免重複初始化。適用於創建成本高的物件。

#### **2.7.1 武器克隆**

**位置：** [include/Weapon/Weapon.hpp](include/Weapon/Weapon.hpp#L72-L74)

```cpp
class Weapon : public nGameObject {
public:
    // 支持克隆複製
    virtual std::shared_ptr<Weapon> Clone() const = 0;
};
```

**具體實現 - GunWeapon：**
```cpp
class GunWeapon final : public Weapon {
public:
    std::shared_ptr<Weapon> Clone() const override {
        return std::make_shared<GunWeapon>(*this);  // 複製構造
    }
};
```

**使用場景：**
```cpp
// 從 JSON 創建原型武器
auto prototypeGun = WeaponFactory::createWeapon(101);

// 商店展示（克隆而非共享）
auto shopDisplayGun = prototypeGun->Clone();

// 玩家拾取（克隆獨立實例）
auto playerGun = prototypeGun->Clone();
playerGun->SetOwner(player);
```

**優點：**
- ✅ 避免重複從 JSON 讀取和初始化
- ✅ 每個玩家獨立武器實例（不共享狀態）
- ✅ 快速創建相同配置的武器

#### **2.7.2 敵人克隆**

**位置：** [include/Factory/CharacterFactory.hpp](include/Factory/CharacterFactory.hpp#L23)

```cpp
class CharacterFactory final : public Factory {
    std::shared_ptr<Character> CloneEnemy(const std::shared_ptr<Character>& original);
};
```

**使用場景：** Boss 召喚小怪
```cpp
// Boss 技能5：雪人召喚
void BossAttackStrategy::ExecuteSkill5(const EnemyContext& ctx) {
    // 創建原型小怪
    auto prototypeMinion = CharacterFactory::GetInstance().createEnemy(201);

    // 克隆多個
    for (int i = 0; i < 3; i++) {
        auto minion = CharacterFactory::GetInstance().CloneEnemy(prototypeMinion);
        minion->SetPosition(bossPos + offset[i]);
        room->AddCharacter(minion);
    }
}
```

---

### 2.8 狀態模式 (State Pattern)

#### **定義與用途**
狀態模式允許物件在內部狀態改變時改變其行為。物件看起來好像修改了它的類。

#### **2.8.1 角色狀態**

**位置：** [include/EnumTypes.hpp](include/EnumTypes.hpp#L42-L53)

```cpp
enum class State {
    STANDING,   // 站立
    MOVING,     // 移動
    SKILL,      // 使用技能
    ATTACK,     // 攻擊
    DEAD        // 死亡
};
```

**狀態管理 - StateComponent：**
```cpp
class StateComponent : public Component {
public:
    void SetState(State newState) {
        if (m_currentState != newState) {
            m_previousState = m_currentState;
            m_currentState = newState;
            OnStateChanged();  // 狀態轉換處理
        }
    }

private:
    void OnStateChanged() {
        // 根據新狀態執行邏輯
        switch (m_currentState) {
            case State::MOVING:
                // 播放移動動畫
                break;
            case State::ATTACK:
                // 播放攻擊動畫
                break;
            case State::DEAD:
                // 停止所有輸入、播放死亡動畫
                break;
        }
    }

    State m_currentState = State::STANDING;
    State m_previousState = State::STANDING;
};
```

#### **2.8.2 敵人 AI 狀態**

**位置：** [include/EnumTypes.hpp](include/EnumTypes.hpp#L125-L136)

```cpp
enum class enemyState {
    IDLE,           // 空閒
    WANDERING,      // 徘徊
    CHASING,        // 追逐
    READY_ATTACK,   // 準備攻擊
    SKILL1,         // 技能1
    SKILL2,         // 技能2
    SKILL3,         // 技能3
    SKILL4,         // 技能4
    SKILL5          // 技能5（Boss）
};
```

**狀態轉換邏輯 - AIComponent：**
```cpp
void AIComponent::Update() {
    switch (m_state) {
        case enemyState::IDLE:
            m_moveStrategy->Update(ctx, deltaTime);  // NoMove
            if (PlayerInRange()) {
                SetState(enemyState::CHASING);
            }
            break;

        case enemyState::WANDERING:
            m_moveStrategy->Update(ctx, deltaTime);  // WanderMove
            if (PlayerInRange()) {
                SetState(enemyState::CHASING);
            }
            break;

        case enemyState::CHASING:
            m_moveStrategy->Update(ctx, deltaTime);  // ChaseMove
            if (m_attackStrategy->CanAttack(ctx)) {
                SetState(enemyState::READY_ATTACK);
            }
            if (!PlayerInRange()) {
                SetState(enemyState::WANDERING);
            }
            break;

        case enemyState::READY_ATTACK:
            m_attackStrategy->Update(ctx, deltaTime);
            SetState(enemyState::CHASING);  // 攻擊後返回追逐
            break;

        // Boss 多技能狀態
        case enemyState::SKILL1:
        case enemyState::SKILL2:
        case enemyState::SKILL3:
        case enemyState::SKILL4:
        case enemyState::SKILL5:
            ExecuteBossSkill();
            break;
    }
}
```

#### **2.8.3 房間狀態**

**位置：** [include/Room/DungeonRoom.hpp](include/Room/DungeonRoom.hpp#L31-L37)

```cpp
enum class RoomState {
    UNEXPLORED,     // 未探索
    EXPLORED,       // 已探索
    COMBAT,         // 戰鬥中
    CLEANED         // 已清理
};
```

**狀態行為：**
```cpp
class DungeonRoom : public Room {
    void SetState(RoomState newState) {
        m_state = newState;
        OnStateChanged();
    }

    virtual void OnStateChanged() {
        switch (m_state) {
            case RoomState::UNEXPLORED:
                // 隱藏房間內容
                break;
            case RoomState::EXPLORED:
                // 顯示地圖，但門未開啟
                break;
            case RoomState::COMBAT:
                // 鎖門、生成敵人
                CloseDoors();
                SpawnEnemies();
                break;
            case RoomState::CLEANED:
                // 開門、掉落獎勵
                OpenDoors();
                SpawnRewards();
                break;
        }
    }
};
```

**優點：**
- ✅ 狀態轉換邏輯集中管理
- ✅ 每個狀態有明確的行為
- ✅ 避免複雜的 flag 組合

---

## 三、可改進之處

### 3.1 高優先級問題

#### **3.1.1 封裝違規 - 公有數據成員**

**位置：** [include/Override/nGameObject.hpp](include/Override/nGameObject.hpp#L24)

**問題代碼：**
```cpp
class nGameObject : public Util::GameObject {
public:
    glm::vec2 m_WorldCoord = {0, 0};  // ❌ 公有數據成員

    // 明明有正確的 getter/setter
    void SetWorldCoord(const glm::vec2 coord) {
        m_WorldCoord = coord + m_PosOffset;  // 偏移補償
    }
    [[nodiscard]] glm::vec2 GetWorldCoord() const { return m_WorldCoord; }

protected:
    glm::vec2 m_PosOffset = glm::vec2(0.0f);
};
```

**影響：**
```cpp
// 錯誤用法：直接修改，繞過偏移補償
gameObject.m_WorldCoord = newPosition;  // Bug！沒有應用 m_PosOffset

// 正確用法：通過 setter
gameObject.SetWorldCoord(newPosition);  // ✅ 正確應用偏移
```

**建議修復：**
```cpp
class nGameObject : public Util::GameObject {
public:
    void SetWorldCoord(const glm::vec2 coord) {
        m_WorldCoord = coord + m_PosOffset;
    }
    [[nodiscard]] glm::vec2 GetWorldCoord() const { return m_WorldCoord; }

protected:  // ✅ 移到 protected
    glm::vec2 m_WorldCoord = {0, 0};
    glm::vec2 m_PosOffset = glm::vec2(0.0f);
};
```

**優先級：** ⚠️ **高** - 可能導致位置計算錯誤

---

#### **3.1.2 缺少輸入驗證 - Weapon Setters**

**位置：** [include/Weapon/Weapon.hpp](include/Weapon/Weapon.hpp#L58-L63)

**問題代碼：**
```cpp
void SetOffset(const int offset) {
    m_offset = offset;  // ❌ 沒有驗證範圍
}

void SetAttackDelay(float delay) {
    m_attackDelay = delay;  // ❌ 沒有驗證 >= 0
    lastAttackTime = m_attackDelay;
}
```

**潛在問題：**
```cpp
weapon->SetOffset(999);      // 過大的偏移
weapon->SetOffset(-999);     // 負數偏移
weapon->SetAttackDelay(-1.0f);  // 負延遲導致立即攻擊
```

**建議修復：**
```cpp
void SetOffset(const int offset) {
    // 限制在合理範圍 -180° 到 180°
    m_offset = std::clamp(offset, -180, 180);
}

void SetAttackDelay(float delay) {
    // 確保非負
    m_attackDelay = std::max(0.0f, delay);
    lastAttackTime = m_attackDelay;
}
```

**優先級：** ⚠️ **高** - 可能導致遊戲邏輯錯誤

---

#### **3.1.3 公有靜態資源 - Projectile**

**位置：** [include/Attack/Projectile.hpp](include/Attack/Projectile.hpp#L43)

**問題代碼：**
```cpp
class Projectile : public Attack {
public:
    static std::unordered_map<std::string, std::shared_ptr<Util::Image>> sharedImages;  // ❌ 公有
};
```

**問題：**
- ❌ 外部代碼可以直接修改/清空圖片緩存
- ❌ 破壞封裝，資源管理邏輯外洩

**建議修復：**
```cpp
class Projectile : public Attack {
public:
    static std::shared_ptr<Util::Image> GetSharedImage(const std::string& path);

private:
    static std::unordered_map<std::string, std::shared_ptr<Util::Image>> sharedImages;  // ✅ 私有
};

// 實現
std::shared_ptr<Util::Image> Projectile::GetSharedImage(const std::string& path) {
    auto it = sharedImages.find(path);
    if (it != sharedImages.end()) {
        return it->second;
    }
    // 載入並緩存
    auto image = std::make_shared<Util::Image>(path);
    sharedImages[path] = image;
    return image;
}
```

**優先級：** ⚠️ **高** - 資源管理安全性問題

---

### 3.2 中優先級問題

#### **3.2.1 代碼重複 - CombatManager**

**位置：**
- [include/Room/BossRoom.hpp](include/Room/BossRoom.hpp#L88-L114)
- [include/Room/MonsterRoom.hpp](include/Room/MonsterRoom.hpp#L80-L135)

**問題：** 兩個房間都有幾乎相同的 `CombatManager` 嵌套類

**BossRoom::CombatManager：**
```cpp
class BossRoom : public DungeonRoom {
    class CombatManager {
    public:
        void StartCombat();
        void EndCombat();
        bool IsInCombat() const;
        bool IsCompleted() const;
    private:
        bool m_isInCombat = false;
        bool m_isCompleted = false;
    };
};
```

**MonsterRoom::CombatManager：**
```cpp
class MonsterRoom : public DungeonRoom {
    class CombatManager {  // ❌ 幾乎相同的代碼
    public:
        void StartCombat();
        void EndCombat();
        bool IsInCombat() const;
        bool IsCompleted() const;
    private:
        bool m_isInCombat = false;
        bool m_isCompleted = false;
    };
};
```

**建議修復方案 1 - 提取基類：**
```cpp
// 新檔案：include/Room/CombatManager.hpp
class CombatManager {
public:
    void StartCombat() {
        m_isInCombat = true;
        OnCombatStart();  // 鉤子方法
    }

    void EndCombat() {
        m_isInCombat = false;
        m_isCompleted = true;
        OnCombatEnd();  // 鉤子方法
    }

    bool IsInCombat() const { return m_isInCombat; }
    bool IsCompleted() const { return m_isCompleted; }

protected:
    virtual void OnCombatStart() {}  // 子類可覆寫
    virtual void OnCombatEnd() {}

private:
    bool m_isInCombat = false;
    bool m_isCompleted = false;
};

// BossRoom 使用組合
class BossRoom : public DungeonRoom {
    CombatManager m_combatManager;
};

// MonsterRoom 使用組合
class MonsterRoom : public DungeonRoom {
    CombatManager m_combatManager;
};
```

**建議修復方案 2 - 介面：**
```cpp
class ICombatManager {
public:
    virtual void StartCombat() = 0;
    virtual void EndCombat() = 0;
    virtual bool IsInCombat() const = 0;
    virtual bool IsCompleted() const = 0;
};

class StandardCombatManager : public ICombatManager {
    // 標準實現
};

class BossCombatManager : public ICombatManager {
    // Boss 特殊邏輯（例如階段切換）
};
```

**優先級：** ⚡ **中** - 維護性問題，不影響功能

---

#### **3.2.2 缺少存取器 - FollowerComponent**

**位置：** [include/Components/FollowerComponent.hpp](include/Components/FollowerComponent.hpp#L62-L65)

**問題代碼：**
```cpp
class FollowerComponent : public Component {
protected:
    glm::vec2 m_RotationLimits = glm::vec2(-glm::pi<float>(), glm::pi<float>());
    glm::vec2 m_HandOffset = glm::vec2(0, 0);
    glm::vec2 m_HoldingOffset = glm::vec2(0, 0);
    // ❌ 沒有 getter/setter
};
```

**問題：**
- 如果是給子類使用，應該文檔化
- 如果需要外部訪問，應該提供 getter/setter

**建議修復：**
```cpp
class FollowerComponent : public Component {
public:
    // 添加 getter/setter
    [[nodiscard]] glm::vec2 GetRotationLimits() const { return m_RotationLimits; }
    void SetRotationLimits(const glm::vec2& limits) { m_RotationLimits = limits; }

    [[nodiscard]] glm::vec2 GetHandOffset() const { return m_HandOffset; }
    void SetHandOffset(const glm::vec2& offset) { m_HandOffset = offset; }

protected:
    glm::vec2 m_RotationLimits = glm::vec2(-glm::pi<float>(), glm::pi<float>());
    glm::vec2 m_HandOffset = glm::vec2(0, 0);
    glm::vec2 m_HoldingOffset = glm::vec2(0, 0);
};
```

**優先級：** ⚡ **中** - API 完整性問題

---

### 3.3 低優先級建議

#### **3.3.1 類型識別改進**

**當前：** 使用字符串識別類型
```cpp
virtual std::string GetName() const override { return "Projectile"; }

if (object->GetName() == "Projectile") {
    // 類型特定邏輯
}
```

**問題：**
- ❌ 字符串比較效率低
- ❌ 容易拼寫錯誤（編譯器不檢查）
- ❌ 不支持多型行為

**建議改進 - 使用枚舉：**
```cpp
enum class GameObjectType {
    PROJECTILE,
    EFFECT_ATTACK,
    CHARACTER,
    WEAPON,
    // ...
};

class nGameObject {
    virtual GameObjectType GetObjectType() const = 0;
};

class Projectile : public Attack {
    GameObjectType GetObjectType() const override {
        return GameObjectType::PROJECTILE;
    }
};

// 使用
if (object->GetObjectType() == GameObjectType::PROJECTILE) {
    auto projectile = std::static_pointer_cast<Projectile>(object);
}
```

**優先級：** 💡 **低** - 優化建議，當前實現可用

---

#### **3.3.2 觀察者模式整合**

**當前：** 多處重複實現觀察者模式
- `InputObserver` / `InputManager`
- `TrackingObserver` / `TrackingManager`
- 自定義事件系統

**建議：** 使用模板統一觀察者實現
```cpp
template <typename EventType>
class Observable {
public:
    void AddObserver(std::function<void(const EventType&)> callback) {
        m_observers.push_back(callback);
    }

    void NotifyObservers(const EventType& event) {
        for (auto& observer : m_observers) {
            observer(event);
        }
    }

private:
    std::vector<std::function<void(const EventType&)>> m_observers;
};

// 使用
class InputManager : public Observable<InputEvent> {
    void ProcessInput(const std::set<char>& keys) {
        NotifyObservers(InputEvent{keys});
    }
};
```

**優點：**
- ✅ 減少代碼重複
- ✅ 型別安全
- ✅ 更靈活（支持 lambda）

**優先級：** 💡 **低** - 重構建議，當前實現穩定

---

## 四、優秀實踐總結

### 4.1 智能指標使用

**✅ 廣泛使用 `std::shared_ptr`：**
```cpp
std::shared_ptr<Character> player;
std::shared_ptr<Weapon> weapon;
std::shared_ptr<Component> component;
```

**✅ 使用 `std::weak_ptr` 防止循環引用：**
```cpp
class Component {
private:
    std::weak_ptr<nGameObject> m_Owner;  // 不增加引用計數
};
```

**✅ 使用 `std::unique_ptr` 表示唯一所有權：**
```cpp
class DungeonRoom {
private:
    std::unique_ptr<GridSystem> m_GridSystem;  // 唯一擁有
};
```

**✅ 使用 `std::enable_shared_from_this`：**
```cpp
class nGameObject : public std::enable_shared_from_this<nGameObject> {
    std::shared_ptr<nGameObject> GetSharedPtr() {
        return shared_from_this();  // 安全獲取 shared_ptr
    }
};
```

---

### 4.2 Modern C++ 特性

**✅ `[[nodiscard]]` 屬性：**
```cpp
[[nodiscard]] int GetMaxHp() const { return m_maxHp; }
// 強制使用返回值，防止：
GetMaxHp();  // 編譯器警告
```

**✅ `= delete` 禁止複製：**
```cpp
Character(const Character &) = delete;
Character &operator=(const Character &) = delete;
```

**✅ `= default` 默認實現：**
```cpp
virtual ~IAttackStrategy() = default;
```

**✅ `final` 關鍵字：**
```cpp
class Character final : public nGameObject {
    // 防止進一步繼承
};

void Update() override final {
    // 防止子類覆寫
}
```

**✅ 可變參數模板：**
```cpp
template <typename T, typename... Args>
std::shared_ptr<T> AddComponent(ComponentType type, Args &&...args) {
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    // ...
}
```

**✅ 強型別枚舉 (enum class)：**
```cpp
enum class State {
    STANDING, MOVING, ATTACK
};  // 不會污染外部命名空間
```

---

### 4.3 RAII 原則

**✅ 資源自動管理：**
```cpp
class DungeonRoom {
    std::unique_ptr<GridSystem> m_GridSystem;
    // 析構時自動釋放，無需手動 delete
};
```

**✅ 智能指標自動管理生命週期：**
```cpp
{
    auto player = std::make_shared<Character>();
    // 作用域結束，自動釋放
}
```

---

### 4.4 組合優於繼承

**✅ 使用組件系統：**
```cpp
// 不是這樣：
class PlayerCharacter : public Character { /* ... */ };
class EnemyCharacter : public Character { /* ... */ };

// 而是這樣：
auto player = std::make_shared<Character>();
player->AddComponent<InputComponent>();  // 玩家特有

auto enemy = std::make_shared<Character>();
enemy->AddComponent<AIComponent>();  // 敵人特有
```

---

### 4.5 介面隔離

**✅ 小而專注的介面：**
```cpp
class IManager {
    virtual void Update() = 0;  // 只有一個職責
};

class IAttackStrategy {
    virtual void Update(...) = 0;
    virtual bool CanAttack(...) = 0;
    virtual float GetAttackDistance() const = 0;
};
```

---

### 4.6 依賴注入

**✅ 策略通過參數傳入：**
```cpp
void SetMoveStrategy(std::shared_ptr<IMoveStrategy> strategy) {
    m_moveStrategy = strategy;
}

void SetAttackStrategy(AttackStrategies type, std::shared_ptr<IAttackStrategy> strategy) {
    m_attackStrategy[type] = strategy;
}
```

---

### 4.7 數據驅動設計

**✅ JSON 配置分離：**
```cpp
// 不是硬編碼：
Character goblin;
goblin.SetMaxHp(30);
goblin.SetDamage(5);
// ...

// 而是從 JSON 讀取：
auto goblin = CharacterFactory::GetInstance().createEnemy(101);
// 所有屬性從 enemy.json 讀取
```

---

## 五、統計數據

### 5.1 類別統計

- **總類別數：** 119+ 個類
- **抽象類/介面：** 15+ 個
  - `IManager`, `IAttackStrategy`, `IMoveStrategy`, `IUtilityStrategy`
  - `ITriggerStrategy`, `IDestructionEffect`
  - `Scene`, `Attack`, `Weapon`, `Skill`, `AreaShape`
  - `InputObserver`, `TrackingObserver`
  - `Component`, `Room`

- **具體類：** 100+ 個
- **Final 類：** 60+ 個（防止進一步繼承）

### 5.2 繼承層次

**最深繼承層次：** 3-4 層
```
Util::GameObject → nGameObject → Attack → Projectile
Util::GameObject → nGameObject → Weapon → GunWeapon (final)
```

**組件層次：** 2 層
```
Component → HealthComponent (final)
Component → AIComponent, public TrackingObserver (多重繼承)
```

**房間層次：** 3 層
```
Room → DungeonRoom → MonsterRoom (final)
Room → DungeonRoom → BossRoom (final)
```

### 5.3 設計模式數量

| 設計模式 | 使用次數 | 位置 |
|---------|---------|------|
| 策略模式 | 4 組 | AttackStrategy, MoveStrategy, TriggerStrategy, DestructionEffect |
| 觀察者模式 | 3 個 | InputObserver, TrackingObserver, EventManager |
| 工廠模式 | 4 個 | CharacterFactory, WeaponFactory, RoomFactory, RoomObjectFactory |
| 單例模式 | 3 個 | ImagePoolManager, SceneManager, EventManager |
| 組件模式 | 1 套系統 | Component + 20+ 組件類型 |
| 物件池模式 | 2 個 | ProjectilePool, EffectAttackPool |
| 原型模式 | 2 處 | Weapon::Clone(), Character 克隆 |
| 狀態模式 | 3 處 | State, enemyState, RoomState |

### 5.4 多型使用

- **純虛擬函數：** 30+ 個
- **虛擬函數覆寫 (override)：** 318+ 處
- **final 修飾符：** 60+ 處

### 5.5 封裝統計

- **訪問修飾符聲明：** 274 處
- **使用 private/protected/public 的檔案：** 87+ 個
- **[[nodiscard]] 使用：** 200+ 處
- **封裝違規：** 1 處（`nGameObject::m_WorldCoord`）

### 5.6 組件類型統計

**20+ 組件類型：**
1. HealthComponent
2. AttackComponent
3. StateComponent
4. AIComponent
5. InputComponent
6. MovementComponent
7. AnimationComponent
8. CollisionComponent
9. SkillComponent
10. TalentComponent
11. ProjectileComponent
12. FollowerComponent
13. TriggerComponent
14. DestructibleComponent
15. InteractableComponent
16. RenderComponent
17. PhysicsComponent
18. SoundComponent
19. ParticleComponent
20. ... 等等

---

## 總結

本專案 **Soul Knight** 展示了優秀的 OOP 設計：

**✅ 優勢：**
1. **深入應用 OOP 四大原則**（繼承、多型、封裝、抽象）
2. **8 種設計模式**廣泛使用，代碼結構清晰
3. **組件化架構**提供極高靈活性，易於擴展
4. **現代 C++ 特性**全面應用（智能指標、`final`、`[[nodiscard]]`）
5. **數據驅動設計**，配置與代碼分離
6. **接口隔離良好**，職責單一

**⚠️ 需改進：**
1. **高優先級：** 修復 `nGameObject::m_WorldCoord` 公有成員
2. **高優先級：** 添加 Setters 輸入驗證
3. **中優先級：** 提取重複的 `CombatManager` 邏輯
4. **低優先級：** 考慮統一觀察者模式實現