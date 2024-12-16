#include "SpiritManager.h"
#include "cocos2d.h"
#include <limits.h>

// 构造函数
SpiritManager::SpiritManager() : nearestEnemy(nullptr) {}

void SpiritManager::init(BlockManager* bm, Player* theplayer) {
    blockManager = bm;
    player = theplayer;
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

// 获取敌人到玩家的距离映射
const std::unordered_map<Enemy*, float>& SpiritManager::getEnemyDistances() const {
    return enemyDistances;
}

// 更新玩家和区块内怪物的状态
void SpiritManager::update(float deltatime) {
    if (player == nullptr) {
        CCLOG("Player is not set!");
        return;
    }

    if (blockManager) {
        // 更新区块内怪物状态，加载与卸载区块
        blockManager->updateBlocksForPlayer(player);
    }
    // 如果需要，更新玩家周围的怪物的其他状态（如AI等）
    updateDistances();
    updateEnemy(deltatime);
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
    std::pair<int, int> playerBlock = blockManager->getBlockCoordinates(playerPos);

    // 获取玩家所在区块内的所有敌人
    std::vector<Enemy*> enemiesInBlock = blockManager->getEnemiesInBlock(playerBlock);

    // 存储最近的怪物和最短距离
    Enemy* nearest = nullptr;
    float minDistance = FLT_MAX;

    // 更新敌人到玩家的距离映射
    enemyDistances.clear(); // 清空旧的距离数据
    for (auto it = enemiesInBlock.begin(); it != enemiesInBlock.end();) {
        Enemy* enemy = *it;
        if (!enemy->getIsAlive()) {
            // 删除敌人并更新迭代器
            it = enemiesInBlock.erase(it);
            continue; // 如果敌人已死，跳过这次循环，继续检查下一个敌人
        }

        // 计算玩家到怪物的距离
        float distance = playerPos.distance(enemy->getPosition());

        // 更新距离映射
        enemyDistances[enemy] = distance;

        // 找到最近的怪物
        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;
        }

        // 增加迭代器到下一个元素
        ++it;
    }

    // 更新最近的怪物
    if (nearest != nearestEnemy) {
        nearestEnemy = nearest;
    }
}

void SpiritManager::updateEnemy(float deltatime){
    if (nearestEnemy) {
        // 遍历所有的敌人和它们到玩家的距离
        for (const auto& entry : enemyDistances) {
            Enemy* enemy = entry.first;
            float distance = entry.second;


            enemy->update(deltatime);
            // 调用怪物的 AI 行为，并传入玩家的等级和敌人距离
            enemy->aiBehavior(distance, player);

        }
    }
}
