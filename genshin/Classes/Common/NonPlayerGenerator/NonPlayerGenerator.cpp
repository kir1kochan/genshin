#include "NonPlayerGenerator.h"
#include "json/document.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>

// ���ɳ����������
SceneObject* NonPlayerGenerator::generateSceneObjectFromJson(const cocos2d::Vec2& position) {
    // ����λ���Ƿ��ж�Ӧ�� SceneObject JSON �ַ���
    if (positionToSceneObjectJsonMapping.find(position) != positionToSceneObjectJsonMapping.end()) {
        std::string jsonString = positionToSceneObjectJsonMapping[position];

        // ����� JSON �ַ����Ѿ������� SceneObject ʵ����ֱ�ӷ��ظ���
        if (jsonToSceneObjectMapping.find(jsonString) != jsonToSceneObjectMapping.end()) {
            SceneObject* existingSO = jsonToSceneObjectMapping[jsonString];
            SceneObject* newSO = existingSO->clone(position);  // ʹ��ָ��λ�ÿ�¡
            return newSO;
        }

        // ���򣬸��� JSON �ַ��������µ� SceneObject ʵ��
        SceneObject* newSO = new SceneObject();
        newSO->loadFromJson(jsonString);  // �� JSON �ַ�����������
        newSO->setPosition(position);     // ������λ��

        // ���µ� SceneObject ʵ������ӳ��
        jsonToSceneObjectMapping[jsonString] = newSO;

        return newSO;
    }
    return nullptr;  // ���û�ж�Ӧ�� JSON �ַ������򷵻ؿ�ָ��
}

// ���ɵ��˶���
Enemy* NonPlayerGenerator::generateEnemyFromJson(const cocos2d::Vec2& position) {
    // ����λ���Ƿ��ж�Ӧ�� Enemy JSON �ַ���
    if (positionToEnemyJsonMapping.find(position) != positionToEnemyJsonMapping.end()) {
        std::string jsonString = positionToEnemyJsonMapping[position];

        // ����� JSON �ַ����Ѿ������� Enemy ʵ����ֱ�ӷ��ظ���
        if (jsonToEnemyMapping.find(jsonString) != jsonToEnemyMapping.end()) {
            Enemy* existingEnemy = jsonToEnemyMapping[jsonString];
            Enemy* newEnemy = existingEnemy->clone(position);  // ʹ��ָ��λ�ÿ�¡
            return newEnemy;
        }

        // ���򣬸��� JSON �ַ��������µ� Enemy ʵ��
        Enemy* newEnemy = new Enemy();
        newEnemy->loadFromJson(jsonString);  // �� JSON �ַ�����������
        newEnemy->setPosition(position);     // ������λ��

        // ���µ� Enemy ʵ������ӳ��
        jsonToEnemyMapping[jsonString] = newEnemy;

        return newEnemy;
    }
    return nullptr;  // ���û�ж�Ӧ�� JSON �ַ������򷵻ؿ�ָ��
}

// �� JSON �ļ����ط�������ݣ�������������͵��ˣ�
void NonPlayerGenerator::loadNonPlayerDataFromJson(const std::string& jsonFilePath) {
    // �򿪲����� JSON �ļ�
    std::ifstream file(jsonFilePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonString = buffer.str();
        file.close();

        // ʹ�� rapidjson ���� JSON �ַ���
        rapidjson::Document doc;
        doc.Parse(jsonString.c_str());

        if (doc.HasParseError()) {
            CCLOG("Error parsing JSON file: %s", jsonFilePath.c_str());
            return;
        }

        // ���� sceneObjects
        if (doc.HasMember("sceneObjects")) {
            const rapidjson::Value& sceneObjects = doc["sceneObjects"];
            for (rapidjson::SizeType i = 0; i < sceneObjects.Size(); ++i) {
                const rapidjson::Value& obj = sceneObjects[i];
                if (obj.HasMember("x") && obj.HasMember("y") && obj.HasMember("json") && obj.HasMember("type")) {
                    cocos2d::Vec2 position(obj["x"].GetFloat(), obj["y"].GetFloat());
                    std::string jsonStr = obj["json"].GetString();
                    std::string type = obj["type"].GetString();

                    // ���� type �ж������� SceneObject
                    if (type == "building" || type == "tree" || type == "otherType") {
                        // �洢λ�õ� SceneObject JSON �ַ�����ӳ��
                        positionToSceneObjectJsonMapping[position] = jsonStr;
                    }
                }
            }
        }

        // ���� enemies
        if (doc.HasMember("enemys")) {
            const rapidjson::Value& enemies = doc["enemys"];
            for (rapidjson::SizeType i = 0; i < enemies.Size(); ++i) {
                const rapidjson::Value& obj = enemies[i];
                if (obj.HasMember("x") && obj.HasMember("y") && obj.HasMember("json")) {
                    cocos2d::Vec2 position(obj["x"].GetFloat(), obj["y"].GetFloat());
                    std::string jsonStr = obj["json"].GetString();

                    // �洢λ�õ� Enemy JSON �ַ�����ӳ��
                    positionToEnemyJsonMapping[position] = jsonStr;
                }
            }
        }
    }
    else {
        CCLOG("Failed to open JSON file: %s", jsonFilePath.c_str());
    }
}

// ����������ݣ�������������͵��ˣ����浽 JSON �ļ�
void NonPlayerGenerator::saveNonPlayerDataToJson(const std::string& jsonFilePath) {
    // ����һ�� JSON �ĵ���������Ϸ����
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // ��� sceneObjects ����
    rapidjson::Value sceneObjects(rapidjson::kArrayType);
    for (const auto& pair : positionToSceneObjectJsonMapping) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("x", pair.first.x, allocator);
        obj.AddMember("y", pair.first.y, allocator);
        obj.AddMember("json", rapidjson::Value(pair.second.c_str(), allocator), allocator);
        // ʾ����ʹ�ù̶����ͣ��������ʵ�������Ҫ�Ķ�
        obj.AddMember("type", "building", allocator);
        sceneObjects.PushBack(obj, allocator);
    }
    doc.AddMember("sceneObjects", sceneObjects, allocator);

    // ��� enemies ����
    rapidjson::Value enemies(rapidjson::kArrayType);
    for (const auto& pair : positionToEnemyJsonMapping) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("x", pair.first.x, allocator);
        obj.AddMember("y", pair.first.y, allocator);
        obj.AddMember("json", rapidjson::Value(pair.second.c_str(), allocator), allocator);
        enemies.PushBack(obj, allocator);
    }
    doc.AddMember("enemys", enemies, allocator);

    // ���� JSON �ļ�
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

// �����������ɵ�ʵ��
void NonPlayerGenerator::clear() {
    // �����������ɵ� SceneObject ʵ��
    for (auto& pair : jsonToSceneObjectMapping) {
        delete pair.second;
    }
    jsonToSceneObjectMapping.clear();

    // �����������ɵ� Enemy ʵ��
    for (auto& pair : jsonToEnemyMapping) {
        delete pair.second;
    }
    jsonToEnemyMapping.clear();
}

// ��ȡ���г��������λ����Ϣ
std::unordered_map<cocos2d::Vec2, std::string> NonPlayerGenerator::getAllSceneObjectPositions() const {
    return positionToSceneObjectJsonMapping;
}

// ��ȡ���е��˵�λ����Ϣ
std::unordered_map<cocos2d::Vec2, std::string> NonPlayerGenerator::getAllEnemyPositions() const {
    return positionToEnemyJsonMapping;
}
