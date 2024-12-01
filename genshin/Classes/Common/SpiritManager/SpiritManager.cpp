#include "SpiritManager.h"
#include "Enemy.h"
#include <unordered_map>
#include <limits.h>

// 构造函数
SpiritManager::SpiritManager() : nearestEnemy(nullptr) {
}

// 设置玩家精灵的指针
void SpiritManager::setPlayer(Player* player) {
    this->player = player;
}

// 获取玩家精灵的指针
Player* SpiritManager::getPlayer() const {
    return player;
}

// 获取最近的怪物
Enemy* SpiritManager::getNearestEnemy() const {
    return nearestEnemy;
}

// 更新玩家和区块内怪物的状态
void SpiritManager::update() {
    if (player == nullptr) {
        CCLOG("Player is not set!");
        return;
    }

    // 更新区块内怪物状态，加载与卸载区块
    blockManager.updateBlocksForPlayer(player);

    // 如果需要，更新玩家周围的怪物的其他状态（如AI等）
    updateDistances();
}

// 更新玩家与怪物之间的最近距离
void SpiritManager::updateDistances() {
    if (player == nullptr) {
        CCLOG("Player is not set!");
        return;
    }

    // 获取玩家的位置
    cocos2d::Vec2 playerPos = player->getPosition();

    // 获取玩家所在的区块
    std::pair<int, int> playerBlock = blockManager.getBlockCoordinates(playerPos);

    // 获取玩家所在区块内的所有敌人
    std::vector<Enemy*> enemiesInBlock = blockManager.getEnemiesInBlock(playerBlock);

    // 寻找最近的怪物
    Enemy* nearest = nullptr;
    float minDistance = FLT_MAX;

    for (Enemy* enemy : enemiesInBlock) {
        // 计算玩家到怪物的距离
        float distance = playerPos.distance(enemy->getPosition());

        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;
        }
    }

    // 更新最近的怪物
    if (nearest != nearestEnemy) {
        nearestEnemy = nearest;
        if (nearestEnemy) {
            CCLOG("Nearest enemy ID: %d, Distance: %.2f", nearestEnemy->getMonsterID(), minDistance);
        }
    }
}
