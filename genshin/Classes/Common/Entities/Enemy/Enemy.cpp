#include "Enemy.h"

// 构造函数
Enemy::Enemy(float health, Element element, int aggressionLevel, float detectionRadius, int baseLevel, const std::string& spriteFilename)
    : Entities(health, attack, defence, element), aggressionLevel(aggressionLevel), detectionRadius(detectionRadius), baseLevel(baseLevel), spriteFilename(spriteFilename), spriteGenerated(false) {}

// 默认构造函数
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE), aggressionLevel(1), detectionRadius(10.0f), baseLevel(1), spriteFilename(""), spriteGenerated(false) {}

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

// 获取精灵文件名
std::string Enemy::getSpriteFilename() const {
    return spriteFilename;
}

// 攻击敌人
void Enemy::attack(Entities& target) {
    CCLOG("Enemy attacking target");
}

// 敌人AI行为
void Enemy::aiBehavior() {
    CCLOG("Enemy AI behavior");
}

// 打印敌人状态
void Enemy::printStatus() {
    Entities::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Sprite Filename: %s", spriteFilename.c_str());
}

// 将敌人对象序列化为 JSON 字符串
std::string Enemy::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    std::string baseJson = Entities::saveToJson();
    rapidjson::Document baseDoc;
    baseDoc.Parse(baseJson.c_str());

    doc.AddMember("health", baseDoc["health"], allocator);
    doc.AddMember("maxHealth", baseDoc["maxHealth"], allocator);
    doc.AddMember("attack", baseDoc["attack"], allocator);
    doc.AddMember("defence", baseDoc["defence"], allocator);
    doc.AddMember("element", baseDoc["element"], allocator);

    doc.AddMember("aggressionLevel", aggressionLevel, allocator);
    doc.AddMember("detectionRadius", detectionRadius, allocator);
    doc.AddMember("baseLevel", baseLevel, allocator);
    doc.AddMember("spriteFilename", rapidjson::Value(spriteFilename.c_str(), allocator), allocator);

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

    Entities::loadFromJson(jsonString);

    if (doc.HasMember("aggressionLevel")) aggressionLevel = doc["aggressionLevel"].GetInt();
    if (doc.HasMember("detectionRadius")) detectionRadius = doc["detectionRadius"].GetFloat();
    if (doc.HasMember("baseLevel")) baseLevel = doc["baseLevel"].GetInt();
    if (doc.HasMember("spriteFilename")) spriteFilename = doc["spriteFilename"].GetString();
}

// 带有新位置的clone
Enemy* Enemy::clone(const cocos2d::Vec2& newPosition) {
    Enemy* newEnemy = new Enemy(health, attack, defence, element, aggressionLevel, detectionRadius, baseLevel, spriteFilename);
    newEnemy->setPosition(newPosition);
    return newEnemy;
}

// 根据文件名生成精灵
cocos2d::Sprite* Enemy::generateSprite() const {
    if (!spriteGenerated) {
        auto sprite = cocos2d::Sprite::create(spriteFilename);
        if (sprite) {
            spriteGenerated = true;  // 精灵生成成功后标记为 true
            return sprite;
        }
    }
    return nullptr;  // 如果精灵已经生成或失败，返回空指针
}