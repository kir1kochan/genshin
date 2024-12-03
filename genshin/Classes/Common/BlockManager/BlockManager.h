#ifndef __BLOCK_MANAGER_H__
#define __BLOCK_MANAGER_H__

#include "cocos2d.h"
#include "NonPlayerGenerator.h"
#include "Enemy.h"
#include "SceneObject.h"
#include <unordered_map>
#include <vector>
#include <set>

// 区块大小定义
#define BLOCK_SIZE 128
#define LOAD_RADIUS 3
#define UNLOAD_RADIUS 4

namespace std {
    template <typename T1, typename T2>
    struct hash<pair<T1, T2>> {
        size_t operator()(const pair<T1, T2>& p) const {
            return hash<T1>()(p.first) ^ hash<T2>()(p.second);
        }
    };
}

class BlockManager {
public:
    BlockManager();
    ~BlockManager();

    // 添加怪物到特定区块
    void addEnemyToBlock(Enemy* enemy);

    // 获取玩家所在的区块
    std::pair<int, int> getBlockCoordinates(const cocos2d::Vec2& position) const;

    // 获取某个区块内的怪物
    std::vector<Enemy*> getEnemiesInBlock(const std::pair<int, int>& blockCoordinates);

    // 获取某个区块内的场景物体
    std::vector<SceneObject*> getSceneObjectsInBlock(const std::pair<int, int>& blockCoordinates);

    // 更新区块内容，加载玩家附近的区块，卸载远离的区块
    void updateBlocksForPlayer(cocos2d::Node* playerNode);

    // 清理区块
    void clear();

private:
    // 用于生成非玩家的场景物体和敌人
    NonPlayerGenerator* nonPlayerGenerator;

    // 存储每个区块中的敌人
    std::unordered_map<std::pair<int, int>, std::vector<Enemy*>, std::hash<int>> blockToEnemies;

    // 存储每个区块中的场景物体
    std::unordered_map<std::pair<int, int>, std::vector<SceneObject*>, std::hash<int>> blockToSceneObjects;

    // 存储已加载的区块
    std::set<std::pair<int, int>> loadedBlocks;

    // 从 NonPlayerGenerator 中加载所有敌人和场景物体位置并存储到区块中
    void loadNonPlayerPositions();

    // 未来的地图加载也考虑纳入其中
};

#endif // __BLOCK_MANAGER_H__
