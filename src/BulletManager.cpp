//
// Created by tjx20 on 3/26/2025.
//

#include "BulletManager.hpp"
#include <execution>

BulletManager& BulletManager::GetInstance() {
	static BulletManager instance;
	return instance;
}

void BulletManager::spawnBullet(const std::string& bulletImagePath, const Util::Transform& transform, glm::vec2 direction, float speed, int damage) {
	bullets.push_back(std::make_shared<Bullet>(bulletImagePath, transform, direction, speed, damage));
}

void BulletManager::Update() {
	float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
	if (bullets.empty()) return;
	// 並行更新
	if (bullets.size() > 100) {  // 仅在大数据时使用并行
		std::for_each(std::execution::par, bullets.begin(), bullets.end(),
			[deltaTime](auto& bullet) { bullet->Update(deltaTime); });
	} else {
		std::for_each(std::execution::seq, bullets.begin(), bullets.end(),
			[deltaTime](auto& bullet) { bullet->Update(deltaTime); });
	}

    // 移除越界子彈
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const auto& bullet) { return bullet->isOutOfBounds(); }),
        bullets.end());
}