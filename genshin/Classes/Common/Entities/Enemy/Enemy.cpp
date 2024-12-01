#include "Enemy.h"

// ���캯��
Enemy::Enemy(float health, Element element, int aggressionLevel, float detectionRadius, int baseLevel, const std::string& spriteFilename)
    : Entities(health, attack, defence, element), aggressionLevel(aggressionLevel), detectionRadius(detectionRadius), baseLevel(baseLevel), spriteFilename(spriteFilename), spriteGenerated(false) {}

// Ĭ�Ϲ��캯��
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE), aggressionLevel(1), detectionRadius(10.0f), baseLevel(1), spriteFilename(""), spriteGenerated(false) {}

// ��ȡ�����Եȼ�
int Enemy::getAggressionLevel() const {
    return aggressionLevel;
}

// ��ȡ���а뾶
float Enemy::getDetectionRadius() const {
    return detectionRadius;
}

// ��ȡ��׼�ȼ�
int Enemy::getBaseLevel() const {
    return baseLevel;
}

// ��ȡ�����ļ���
std::string Enemy::getSpriteFilename() const {
    return spriteFilename;
}

// ��������
void Enemy::attack(Entities& target) {
    CCLOG("Enemy attacking target");
}

// ����AI��Ϊ
void Enemy::aiBehavior() {
    CCLOG("Enemy AI behavior");
}

// ��ӡ����״̬
void Enemy::printStatus() {
    Entities::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Sprite Filename: %s", spriteFilename.c_str());
}

// �����˶������л�Ϊ JSON �ַ���
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

// �� JSON �ַ�����������
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

// ������λ�õ�clone
Enemy* Enemy::clone(const cocos2d::Vec2& newPosition) {
    Enemy* newEnemy = new Enemy(health, attack, defence, element, aggressionLevel, detectionRadius, baseLevel, spriteFilename);
    newEnemy->setPosition(newPosition);
    return newEnemy;
}

// �����ļ������ɾ���
cocos2d::Sprite* Enemy::generateSprite() const {
    if (!spriteGenerated) {
        auto sprite = cocos2d::Sprite::create(spriteFilename);
        if (sprite) {
            spriteGenerated = true;  // �������ɳɹ�����Ϊ true
            return sprite;
        }
    }
    return nullptr;  // ��������Ѿ����ɻ�ʧ�ܣ����ؿ�ָ��
}