#ifndef __SPIRIT_MANAGER_H__
#define __SPIRIT_MANAGER_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Player.h"
#include "BlockManager.h"

class SpiritManager {
private:
    // 玩家精灵的指针
    Player* player = nullptr;

    // 区块管理器，用于管理区块和怪物
    BlockManager blockManager;

    // 存储最近的怪物
    Enemy* nearestEnemy = nullptr;

public:
    // 构造函数
    SpiritManager();

    // 设置玩家精灵的指针
    void setPlayer(Player* player);

    // 获取玩家精灵的指针
    Player* getPlayer() const;

    // 获取最近的怪物
    Enemy* getNearestEnemy() const;

    // 更新玩家和区块内怪物的状态
    void update();

    // 更新玩家与怪物之间的最近距离
    void updateDistances();
};

#endif // __SPIRIT_MANAGER_H__
