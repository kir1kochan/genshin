#ifndef __BLOCK_MANAGER_H__
#define __BLOCK_MANAGER_H__

#include "cocos2d.h"
#include "SceneObject.h"
#include "Enemy.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <utility>

#define BLOCK_SIZE 1280
#define LOAD_RADIUS 2
#define UNLOAD_RADIUS 3

class BlockManager {
public:
    BlockManager();
    ~BlockManager();

    std::pair<int, int> getBlockCoordinates(const cocos2d::Vec2& position) const;
    void updateBlocksForPlayer(Player* playerNode);
    void handleClickEvent(const cocos2d::Vec2& clickPosition);
    void clear();
    
    // 更新玩家所在区域的物体的碰撞箱的映射
    void updateCollisionMap();

    // 获取指定区块的所有敌人
    std::vector<Enemy*> getEnemiesInBlock(const std::pair<int, int>& block);

    // 获取指定区块的所有场景物体
    std::vector<SceneObject*> getSceneObjectsInBlock(const std::pair<int, int>& block);

private:
    struct PairHash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    // 从 TMX 文件加载对象数据
    void loadObjectsFromTMX(const std::string& tmxFile);

    // 处理点击事件
    void checkCollisions(const cocos2d::Vec2& clickPosition);

    // 存储区块状态（true为加载，false为卸载）
    std::unordered_map<std::pair<int, int>, bool, PairHash> blockStatus;

    // 已加载区块集合
    std::set<std::pair<int, int>> loadedBlocks;

    // 存储区块内的敌人
    std::unordered_map<std::pair<int, int>, std::vector<Enemy*>, PairHash> blockToEnemies;

    // 存储区块内的场景物体
    std::unordered_map<std::pair<int, int>, std::vector<SceneObject*>, PairHash> blockToSceneObjects;

    // 玩家当前所在的区块
    std::pair<int, int> playerBlock = { 0, 0 };
    // 存储玩家当前区域的所有SceneObject的碰撞框和实例的映射
    std::unordered_map<std::pair<int, int>, std::unordered_map<SceneObject*, cocos2d::Rect>, PairHash> sceneObjectCollisions;
};


#endif // __BLOCK_MANAGER_H__
