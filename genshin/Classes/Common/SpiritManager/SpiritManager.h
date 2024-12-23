#ifndef __SPIRIT_MANAGER_H__
#define __SPIRIT_MANAGER_H__

#include "cocos2d.h"
#include "../Entities/Enemy/Enemy.h"
#include "../Entities/Player/Player.h"
#include "../BlockManager/BlockManager.h"
#include <unordered_map>

class SpiritManager {
private:
    // 玩家精灵的指针
    Player* player = nullptr;

    // 区块管理器，用于管理区块和怪物
    BlockManager* blockManager = nullptr;

    // 存储最近的怪物
    Enemy* nearestEnemy = nullptr;

    // 存储区块内每个敌人与玩家之间的距离
    std::unordered_map<Enemy*, float> enemyDistances;

public:
    // 构造函数
    SpiritManager();

    void init(BlockManager* bm, Player* theplayer);

    // 设置玩家精灵的指针
    void setPlayer(Player* player);

    // 获取玩家精灵的指针
    Player* getPlayer() const;

    // 获取最近的怪物
    Enemy* getNearestEnemy() const;

    // 获取敌人到玩家的距离映射
    const std::unordered_map<Enemy*, float>& getEnemyDistances() const;

    // 更新玩家和区块内怪物的状态
    void update(float deltatime);

    // 更新玩家与怪物之间的最近距离
    void updateDistances();

    // 更新怪物行为
    void updateEnemy(float deltatime);
};

#endif // __SPIRIT_MANAGER_H__
