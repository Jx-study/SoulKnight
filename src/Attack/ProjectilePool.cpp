//
// Created by tjx20 on 4/18/2025.
//

#include "Attack/ProjectilePool.hpp"
#include "Util/Logger.hpp"

std::deque<std::shared_ptr<Projectile>> ProjectilePool::pool;

std::shared_ptr<Projectile> ProjectilePool::Acquire(const ProjectileInfo& projectileInfo) {
	if (!pool.empty()) {
		// 從池中取出子彈並重置屬性
		auto bullet = pool.back();
		if (!bullet) {
			//LOG_ERROR("bullet from pool is nullptr!");
		}
		bullet->ResetAll(projectileInfo);
		pool.pop_back();  // 從池中移除
		bullet->SetActive(true);   // 激活子彈
		bullet->SetControlVisible(true);
		//LOG_DEBUG("use old bullet");
		return bullet;
	}

	// 如果池是空的，創建一個新的子彈
	auto newBullet = std::make_shared<Projectile>(projectileInfo);
	LOG_DEBUG("created new bullet");
	return newBullet;
}