//
// Created by tjx20 on 3/10/2025.
//

#include "Factory/CharacterFactory.hpp"
#include <fstream>
#include <memory>

#include "Components/AttackComponent.hpp"
#include "Components/InputComponent.hpp"

CharacterFactory::CharacterFactory() {}


State stringToState(const std::string& stateStr) {
	if (stateStr == "STANDING") return State::STANDING;
	if (stateStr == "MOVING") return State::MOVING;
	if (stateStr == "ATTACK") return State::ATTACK;
	if (stateStr == "DEAD") return State::DEAD;
	throw std::invalid_argument("Unknown state: " + stateStr);
}

std::unordered_map<State, std::shared_ptr<Animation>> parseAnimations(const nlohmann::json& animationsJson) {
	std::unordered_map<State, std::shared_ptr<Animation>> animations;
	for (const auto& [key, value] : animationsJson.items()) {
		State state = stringToState(key);
		std::vector<std::string> frames;
		for (const auto& frame : value) {
			frames.push_back(RESOURCE_DIR + frame.get<std::string>());
		}
		animations[state] = std::make_shared<Animation>(frames); // 使用 shared_ptr 包装 Animation
	}
	return animations;
}

std::shared_ptr<Player> CharacterFactory::createPlayer(const int id) {
	// 讀取角色 JSON 資料
	nlohmann::json characterData = readJsonFile("player.json");

	// 在 JSON 陣列中搜尋符合名稱的角色
	for (const auto& characterInfo : characterData) {
		if (characterInfo["ID"] == id) {
			//auto player = std::make_shared<Player>();
			auto animation = parseAnimations(characterInfo["animations"]);

			int maxHp = characterInfo["maxHp"];
			float moveSpeed = characterInfo["speed"];

			// **讀取碰撞箱資訊**
			float collisionWidth = characterInfo["collisionBox"]["width"];
			float collisionHeight = characterInfo["collisionBox"]["height"];
			float offsetX = characterInfo["collisionBox"]["offsetX"];
			float offsetY = characterInfo["collisionBox"]["offsetY"];
			auto collisionBox = std::make_unique<CollisionBox>(offsetX, offsetY, collisionWidth, collisionHeight);

			// 解析武器名稱並創建武器
			const int weaponID = characterInfo["weaponID"];
			auto weapon = wf.createWeapon(weaponID);

			int maxArmor = characterInfo["maxArmor"];
			int maxEnergy = characterInfo["maxEnergy"];
			double criticalRate = characterInfo["criticalRate"];
			int handBladeDamage = characterInfo["handBladeDamage"];

			// 讀取技能
			std::shared_ptr<Skill> skill = nullptr;
			if (characterInfo.contains("skill")) {
				skill = std::make_shared<Skill>(
					characterInfo["skill"]["name"],
					characterInfo["skill"]["cooldown"],
					characterInfo["skill"]["damage"]
				);
			}

			// 根據 JSON 內容來決定創建 Player
			auto player = std::make_shared<Player>(maxHp, moveSpeed, std::move(collisionBox), maxArmor, maxEnergy, skill);

			auto animationComponent = player->AddComponent<AnimationComponent>(ComponentType::ANIMATION, animation);
			auto stateComponent = player->AddComponent<StateComponent>(ComponentType::STATE);
			auto inputComponent = player->AddComponent<InputComponent>(ComponentType::INPUT);
			auto movementComponent = player->AddComponent<MovementComponent>(ComponentType::MOVEMENT);
			auto attackComponent = player->AddComponent<AttackComponent>(ComponentType::ATTACK, criticalRate, handBladeDamage, weapon);
			auto CollisionComp = player->AddComponent<CollisionComponent>(ComponentType::COLLISION);
			CollisionComp->SetCollisionLayer(CollisionLayers_Player);
			CollisionComp->SetCollisionMask(CollisionLayers_None);
			auto FollowerComp = weapon->AddComponent<FollowerComponent>(ComponentType::FOLLOWER);
			FollowerComp->SetFollower(player);
			FollowerComp->IsTargetMouse(true);
			FollowerComp->SetHandOffset(glm::vec2(30/7.0f,-25/4.0f));
			FollowerComp->SetHoldingPosition(glm::vec2(30/2.0f,0));
			// 设置旋转限制 (45度到135度)
			FollowerComp->SetRotationLimits(
				glm::radians(45.0f),
				glm::radians(135.0f)
			);
			return player;
		}
	}
	LOG_DEBUG("{}'s ID not found: {}", id);
	return nullptr;
}

std::shared_ptr<Enemy>CharacterFactory::createEnemy(const int id) {
    // 讀取角色 JSON 資料
	nlohmann::json characterData = readJsonFile("enemy.json");

    // 在 JSON 陣列中搜尋符合名稱的角色
    for (const auto& characterInfo : characterData) {
        if (characterInfo["id"] == id) {
        	auto animation = parseAnimations(characterInfo["animations"]);
            int maxHp = characterInfo["maxHp"];
            float moveSpeed = characterInfo["speed"];
            int aimRange = characterInfo["aimRange"];

        	// **讀取碰撞箱資訊**
        	float collisionWidth = characterInfo["collisionBox"]["width"];
        	float collisionHeight = characterInfo["collisionBox"]["height"];
        	float offsetX = characterInfo["collisionBox"]["offsetX"];
        	float offsetY = characterInfo["collisionBox"]["offsetY"];
        	auto collisionBox = std::make_unique<CollisionBox>(offsetX, offsetY, collisionWidth, collisionHeight);

            // 解析武器名稱並創建武器
            int weaponID = characterInfo["weaponID"];
            auto weapon = wf.createWeapon(weaponID);

            // 根據 JSON 內容來決定創建 Enemy
        	return std::make_shared<Enemy>(maxHp, moveSpeed, std::move(collisionBox));
        }
    }

    LOG_DEBUG("{}'s ID not found: {}", id);
	return nullptr;
}