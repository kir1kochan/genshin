#ifndef NON_PLAYER_MANAGER_H
#define NON_PLAYER_MANAGER_H

#include "cocos2d.h"
#include "SceneObject.h"
#include "Enemy.h"
#include <unordered_map>
#include <string>
#include <vector>

class NonPlayerManager {
public:
    NonPlayerManager() = default;
    ~NonPlayerManager() = default;

    // 获取所有场景物体的位置信息
    std::unordered_map<cocos2d::Vec2, SceneObject*> getAllSceneObjects() const;

    // 获取所有敌人的位置信息
    std::unordered_map<cocos2d::Vec2, Enemy*> getAllEnemies() const;

    // 通过位置获取场景物体
    SceneObject* getSceneObjectAt(const cocos2d::Vec2& position);

    // 通过位置获取敌人
    Enemy* getEnemyAt(const cocos2d::Vec2& position);

    // 添加场景物体
    void addSceneObject(const cocos2d::Vec2& position, SceneObject* object);

    // 添加敌人
    void addEnemy(const cocos2d::Vec2& position, Enemy* enemy);

    // 检查是否与场景物体发生碰撞
    bool checkCollisionWithSceneObject(const cocos2d::Vec2& position, const cocos2d::Rect& rect);

    // 检查是否与敌人发生碰撞
    bool checkCollisionWithEnemy(const cocos2d::Vec2& position, const cocos2d::Rect& rect);

private:
    // 存储场景物体和敌人
    std::unordered_map<cocos2d::Vec2, SceneObject*> sceneObjects;
    std::unordered_map<cocos2d::Vec2, Enemy*> enemies;
};

#endif // NON_PLAYER_MANAGER_H
