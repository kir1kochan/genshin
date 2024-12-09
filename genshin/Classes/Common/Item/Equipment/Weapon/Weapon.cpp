#include "Weapon.h"

// ���캯��
Weapon::Weapon(int id, const std::string& name, int attackPower, float attackRange, float attackSpeed)
    : Equipment(id, name, attackPower), attackRange(attackRange), attackSpeed(attackSpeed) {}

// ��ȡ������Χ
float Weapon::getAttackRange() const {
    return attackRange;
}

// ��ȡ����Ƶ��
float Weapon::getAttackSpeed() const {
    return attackSpeed;
}

// ��д��ӡ��Ϣ����
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
    CCLOG("Attack Range: %.2f", attackRange);
    CCLOG("Attack Speed: %.2f", attackSpeed);
}

// �������������л�Ϊ JSON �ַ���
std::string Weapon::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // ���û��� Equipment �� saveToJson
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("power", power, allocator);

    // ����������е�����
    doc.AddMember("attackRange", attackRange, allocator);
    doc.AddMember("attackSpeed", attackSpeed, allocator);

    // ת��Ϊ JSON �ַ���
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// �� JSON �ַ���������������
void Weapon::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Weapon");
        return;
    }

    // �� JSON �м��ػ�������
    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("power")) power = doc["power"].GetInt();

    // �� JSON �м����������е�����
    if (doc.HasMember("attackRange")) attackRange = doc["attackRange"].GetFloat();
    if (doc.HasMember("attackSpeed")) attackSpeed = doc["attackSpeed"].GetFloat();
}