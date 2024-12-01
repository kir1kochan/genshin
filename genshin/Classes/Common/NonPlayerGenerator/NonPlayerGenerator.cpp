#include "NonPlayerGenerator.h"
#include "json/document.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>

// 生成场景物体对象
SceneObject* NonPlayerGenerator::generateSceneObjectFromJson(const cocos2d::Vec2& position) {
    // 查找位置是否有对应的 SceneObject JSON 字符串
    if (positionToSceneObjectJsonMapping.find(position) != positionToSceneObjectJsonMapping.end()) {
        std::string jsonString = positionToSceneObjectJsonMapping[position];

        // 如果该 JSON 字符串已经生成了 SceneObject 实例，直接返回副本
        if (jsonToSceneObjectMapping.find(jsonString) != jsonToSceneObjectMapping.end()) {
            SceneObject* existingSO = jsonToSceneObjectMapping[jsonString];
            SceneObject* newSO = existingSO->clone(position);  // 使用指定位置克隆
            return newSO;
        }

        // 否则，根据 JSON 字符串创建新的 SceneObject 实例
        SceneObject* newSO = new SceneObject();
        newSO->loadFromJson(jsonString);  // 从 JSON 字符串加载数据
        newSO->setPosition(position);     // 设置新位置

        // 将新的 SceneObject 实例存入映射
        jsonToSceneObjectMapping[jsonString] = newSO;

        return newSO;
    }
    return nullptr;  // 如果没有对应的 JSON 字符串，则返回空指针
}

// 生成敌人对象
Enemy* NonPlayerGenerator::generateEnemyFromJson(const cocos2d::Vec2& position) {
    // 查找位置是否有对应的 Enemy JSON 字符串
    if (positionToEnemyJsonMapping.find(position) != positionToEnemyJsonMapping.end()) {
        std::string jsonString = positionToEnemyJsonMapping[position];

        // 如果该 JSON 字符串已经生成了 Enemy 实例，直接返回副本
        if (jsonToEnemyMapping.find(jsonString) != jsonToEnemyMapping.end()) {
            Enemy* existingEnemy = jsonToEnemyMapping[jsonString];
            Enemy* newEnemy = existingEnemy->clone(position);  // 使用指定位置克隆
            return newEnemy;
        }

        // 否则，根据 JSON 字符串创建新的 Enemy 实例
        Enemy* newEnemy = new Enemy();
        newEnemy->loadFromJson(jsonString);  // 从 JSON 字符串加载数据
        newEnemy->setPosition(position);     // 设置新位置

        // 将新的 Enemy 实例存入映射
        jsonToEnemyMapping[jsonString] = newEnemy;

        return newEnemy;
    }
    return nullptr;  // 如果没有对应的 JSON 字符串，则返回空指针
}

// 从 JSON 文件加载非玩家数据（包括场景物体和敌人）
void NonPlayerGenerator::loadNonPlayerDataFromJson(const std::string& jsonFilePath) {
    // 打开并解析 JSON 文件
    std::ifstream file(jsonFilePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonString = buffer.str();
        file.close();

        // 使用 rapidjson 解析 JSON 字符串
        rapidjson::Document doc;
        doc.Parse(jsonString.c_str());

        if (doc.HasParseError()) {
            CCLOG("Error parsing JSON file: %s", jsonFilePath.c_str());
            return;
        }

        // 处理 sceneObjects
        if (doc.HasMember("sceneObjects")) {
            const rapidjson::Value& sceneObjects = doc["sceneObjects"];
            for (rapidjson::SizeType i = 0; i < sceneObjects.Size(); ++i) {
                const rapidjson::Value& obj = sceneObjects[i];
                if (obj.HasMember("x") && obj.HasMember("y") && obj.HasMember("json") && obj.HasMember("type")) {
                    cocos2d::Vec2 position(obj["x"].GetFloat(), obj["y"].GetFloat());
                    std::string jsonStr = obj["json"].GetString();
                    std::string type = obj["type"].GetString();

                    // 根据 type 判断是生成 SceneObject
                    if (type == "building" || type == "tree" || type == "otherType") {
                        // 存储位置到 SceneObject JSON 字符串的映射
                        positionToSceneObjectJsonMapping[position] = jsonStr;
                    }
                }
            }
        }

        // 处理 enemies
        if (doc.HasMember("enemys")) {
            const rapidjson::Value& enemies = doc["enemys"];
            for (rapidjson::SizeType i = 0; i < enemies.Size(); ++i) {
                const rapidjson::Value& obj = enemies[i];
                if (obj.HasMember("x") && obj.HasMember("y") && obj.HasMember("json")) {
                    cocos2d::Vec2 position(obj["x"].GetFloat(), obj["y"].GetFloat());
                    std::string jsonStr = obj["json"].GetString();

                    // 存储位置到 Enemy JSON 字符串的映射
                    positionToEnemyJsonMapping[position] = jsonStr;
                }
            }
        }
    }
    else {
        CCLOG("Failed to open JSON file: %s", jsonFilePath.c_str());
    }
}

// 将非玩家数据（包括场景物体和敌人）保存到 JSON 文件
void NonPlayerGenerator::saveNonPlayerDataToJson(const std::string& jsonFilePath) {
    // 创建一个 JSON 文档并保存游戏数据
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 添加 sceneObjects 数组
    rapidjson::Value sceneObjects(rapidjson::kArrayType);
    for (const auto& pair : positionToSceneObjectJsonMapping) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("x", pair.first.x, allocator);
        obj.AddMember("y", pair.first.y, allocator);
        obj.AddMember("json", rapidjson::Value(pair.second.c_str(), allocator), allocator);
        // 示例中使用固定类型，这里根据实际情况需要改动
        obj.AddMember("type", "building", allocator);
        sceneObjects.PushBack(obj, allocator);
    }
    doc.AddMember("sceneObjects", sceneObjects, allocator);

    // 添加 enemies 数组
    rapidjson::Value enemies(rapidjson::kArrayType);
    for (const auto& pair : positionToEnemyJsonMapping) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("x", pair.first.x, allocator);
        obj.AddMember("y", pair.first.y, allocator);
        obj.AddMember("json", rapidjson::Value(pair.second.c_str(), allocator), allocator);
        enemies.PushBack(obj, allocator);
    }
    doc.AddMember("enemys", enemies, allocator);

    // 保存 JSON 文件
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::ofstream file(jsonFilePath);
    if (file.is_open()) {
        file << buffer.GetString();
        file.close();
        CCLOG("Non-player data saved to file: %s", jsonFilePath.c_str());
    }
    else {
        CCLOG("Failed to save JSON file: %s", jsonFilePath.c_str());
    }
}

// 清理所有生成的实例
void NonPlayerGenerator::clear() {
    // 清理所有生成的 SceneObject 实例
    for (auto& pair : jsonToSceneObjectMapping) {
        delete pair.second;
    }
    jsonToSceneObjectMapping.clear();

    // 清理所有生成的 Enemy 实例
    for (auto& pair : jsonToEnemyMapping) {
        delete pair.second;
    }
    jsonToEnemyMapping.clear();
}

// 获取所有场景物体的位置信息
std::unordered_map<cocos2d::Vec2, std::string> NonPlayerGenerator::getAllSceneObjectPositions() const {
    return positionToSceneObjectJsonMapping;
}

// 获取所有敌人的位置信息
std::unordered_map<cocos2d::Vec2, std::string> NonPlayerGenerator::getAllEnemyPositions() const {
    return positionToEnemyJsonMapping;
}
