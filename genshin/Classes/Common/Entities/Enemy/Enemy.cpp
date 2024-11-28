#include "Enemy.h"

// ���캯��
Enemy::Enemy(float health, float attack, float defence, Element element, int aggressionLevel, float detectionRadius, int baseLevel, int monsterID)
    : Entities(health, attack, defence, element), aggressionLevel(aggressionLevel), detectionRadius(detectionRadius), baseLevel(baseLevel), monsterID(monsterID) {}

// Ĭ�Ϲ��캯��
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE), aggressionLevel(1), detectionRadius(10.0f), baseLevel(1), monsterID(0) {}

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

// ��ȡ������
int Enemy::getMonsterID() const {
    return monsterID;
}

// ��������
void Enemy::attack(Entities& target) {
    // �����߼�...
    CCLOG("Enemy attacking target");
}

// ����AI��Ϊ
void Enemy::aiBehavior() {
    // AI�߼�...
    CCLOG("Enemy AI behavior");
}

// ��ӡ����״̬
void Enemy::printStatus() {
    Entities::printStatus();  // ��ӡ����ʵ���״̬
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Monster ID: %d", monsterID);
}

// �����˶������л�Ϊ JSON �ַ���
std::string Enemy::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // ���ø���� saveToJson ��ȡ��������
    std::string baseJson = Entities::saveToJson();
    rapidjson::Document baseDoc;
    baseDoc.Parse(baseJson.c_str());

    // �ϲ���������
    doc.AddMember("health", baseDoc["health"], allocator);
    doc.AddMember("maxHealth", baseDoc["maxHealth"], allocator);
    doc.AddMember("attack", baseDoc["attack"], allocator);
    doc.AddMember("defence", baseDoc["defence"], allocator);
    doc.AddMember("element", baseDoc["element"], allocator);

    // ��� Enemy ���е�����
    doc.AddMember("aggressionLevel", aggressionLevel, allocator);
    doc.AddMember("detectionRadius", detectionRadius, allocator);
    doc.AddMember("baseLevel", baseLevel, allocator);
    doc.AddMember("monsterID", monsterID, allocator);

    // ת��Ϊ JSON �ַ���
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

    // ���ø���� loadFromJson �����������
    Entities::loadFromJson(jsonString);

    // ���� Enemy ���е�����
    if (doc.HasMember("aggressionLevel")) aggressionLevel = doc["aggressionLevel"].GetInt();
    if (doc.HasMember("detectionRadius")) detectionRadius = doc["detectionRadius"].GetFloat();
    if (doc.HasMember("baseLevel")) baseLevel = doc["baseLevel"].GetInt();
    if (doc.HasMember("monsterID")) monsterID = doc["monsterID"].GetInt();
}
