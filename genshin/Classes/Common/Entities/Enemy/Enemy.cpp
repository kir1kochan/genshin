#include "Enemy.h"

// 构造函数
Enemy::Enemy(float health, float attack, float defence, Element element, int aggressionLevel, float detectionRadius, int baseLevel, int monsterID)
    : Entities(health, attack, defence, element), aggressionLevel(aggressionLevel), detectionRadius(detectionRadius), baseLevel(baseLevel), monsterID(monsterID) {}

// 默认构造函数
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE), aggressionLevel(1), detectionRadius(10.0f), baseLevel(1), monsterID(0) {}

// 获取攻击性等级
int Enemy::getAggressionLevel() const {
    return aggressionLevel;
}

// 获取索敌半径
float Enemy::getDetectionRadius() const {
    return detectionRadius;
}

// 获取基准等级
int Enemy::getBaseLevel() const {
    return baseLevel;
}

// 获取怪物编号
int Enemy::getMonsterID() const {
    return monsterID;
}

// 攻击敌人
void Enemy::attack(Entities& target) {
    // 攻击逻辑...
    CCLOG("Enemy attacking target");
}

// 敌人AI行为
void Enemy::aiBehavior() {
    // AI逻辑...
    CCLOG("Enemy AI behavior");
}

// 打印敌人状态
void Enemy::printStatus() {
    Entities::printStatus();  // 打印父类实体的状态
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Monster ID: %d", monsterID);
}

// 将敌人对象序列化为 JSON 字符串
std::string Enemy::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 调用父类的 saveToJson 获取基础数据
    std::string baseJson = Entities::saveToJson();
    rapidjson::Document baseDoc;
    baseDoc.Parse(baseJson.c_str());

    // 合并父类数据
    doc.AddMember("health", baseDoc["health"], allocator);
    doc.AddMember("maxHealth", baseDoc["maxHealth"], allocator);
    doc.AddMember("attack", baseDoc["attack"], allocator);
    doc.AddMember("defence", baseDoc["defence"], allocator);
    doc.AddMember("element", baseDoc["element"], allocator);

    // 添加 Enemy 特有的数据
    doc.AddMember("aggressionLevel", aggressionLevel, allocator);
    doc.AddMember("detectionRadius", detectionRadius, allocator);
    doc.AddMember("baseLevel", baseLevel, allocator);
    doc.AddMember("monsterID", monsterID, allocator);

    // 转换为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串加载数据
void Enemy::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // 调用父类的 loadFromJson 处理基础数据
    Entities::loadFromJson(jsonString);

    // 加载 Enemy 特有的数据
    if (doc.HasMember("aggressionLevel")) aggressionLevel = doc["aggressionLevel"].GetInt();
    if (doc.HasMember("detectionRadius")) detectionRadius = doc["detectionRadius"].GetFloat();
    if (doc.HasMember("baseLevel")) baseLevel = doc["baseLevel"].GetInt();
    if (doc.HasMember("monsterID")) monsterID = doc["monsterID"].GetInt();
}
