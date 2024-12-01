#ifndef NON_PLAYER_GENERATOR_H
#define NON_PLAYER_GENERATOR_H

#include "cocos2d.h"
#include "SceneObject.h"
#include "Enemy.h"
#include <unordered_map>
#include <string>

class NonPlayerGenerator {
public:
    NonPlayerGenerator() = default;
    ~NonPlayerGenerator() = default;

    // 生成场景物体对象
    SceneObject* generateSceneObjectFromJson(const cocos2d::Vec2& position);

    // 生成敌人对象
    Enemy* generateEnemyFromJson(const cocos2d::Vec2& position);

    // 从 JSON 文件加载非玩家数据（包括场景物体和敌人）
    void loadNonPlayerDataFromJson(const std::string& jsonFilePath);

    // 将非玩家数据（包括场景物体和敌人）保存到 JSON 文件
    void saveNonPlayerDataToJson(const std::string& jsonFilePath);

    // 获取所有场景物体的位置信息
    std::unordered_map<cocos2d::Vec2, std::string> getAllSceneObjectPositions() const;

    // 获取所有敌人的位置信息
    std::unordered_map<cocos2d::Vec2, std::string> getAllEnemyPositions() const;

    // 清理所有生成的实例
    void clear();

private:
    // 存储位置到 SceneObject JSON 字符串的映射
    std::unordered_map<cocos2d::Vec2, std::string> positionToSceneObjectJsonMapping;

    // 存储位置到 Enemy JSON 字符串的映射
    std::unordered_map<cocos2d::Vec2, std::string> positionToEnemyJsonMapping;

    // 存储已生成的 SceneObject 实例
    std::unordered_map<std::string, SceneObject*> jsonToSceneObjectMapping;

    // 存储已生成的 Enemy 实例
    std::unordered_map<std::string, Enemy*> jsonToEnemyMapping;
};

#endif // NON_PLAYER_GENERATOR_H
