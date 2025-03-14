//
// Created by tjx20 on 3/10/2025.
//

#ifndef CHARACTERFACTORY_HPP
#define CHARACTERFACTORY_HPP

#include "Creature/Character.hpp"
#include "Creature/Player.hpp"
#include "Creature/Enemy.hpp"
#include "Weapon/Weapon.hpp"
#include "Tool/CollisionBox.hpp"
#include <memory>
#include <string>
#include <json.hpp>

// 角色工廠：根據名稱創建角色
class CharacterFactory {
public:
	// 根據角色配置文件創建角色
	std::shared_ptr<Player> createPlayer(const int id);
	std::shared_ptr<Enemy> createEnemy(const int id);

// private:
	// 從 JSON 創建武器
	std::shared_ptr<Weapon> createWeapon(const int weaponID);
	// 讀取 JSON 文件
	nlohmann::json readJsonFile(const std::string& filePath);
};

#endif // CHARACTERFACTORY_HPP