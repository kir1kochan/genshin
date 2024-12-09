#include "Weapon.h"

// 构造函数
Weapon::Weapon(int id, const std::string& name, int attackPower, float attackRange, float attackSpeed)
    : Equipment(id, name, attackPower), attackRange(attackRange), attackSpeed(attackSpeed) {}

// 获取攻击范围
float Weapon::getAttackRange() const {
    return attackRange;
}

// 获取攻击频率
float Weapon::getAttackSpeed() const {
    return attackSpeed;
}

// 重写打印信息函数
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
    CCLOG("Attack Range: %.2f", attackRange);
    CCLOG("Attack Speed: %.2f", attackSpeed);
}

// 将武器数据序列化为 JSON 字符串
std::string Weapon::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 调用基类 Equipment 的 saveToJson
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("power", power, allocator);

    // 添加武器特有的属性
    doc.AddMember("attackRange", attackRange, allocator);
    doc.AddMember("attackSpeed", attackSpeed, allocator);

    // 转换为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串加载武器数据
void Weapon::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Weapon");
        return;
    }

    // 从 JSON 中加载基类数据
    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("power")) power = doc["power"].GetInt();

    // 从 JSON 中加载武器特有的属性
    if (doc.HasMember("attackRange")) attackRange = doc["attackRange"].GetFloat();
    if (doc.HasMember("attackSpeed")) attackSpeed = doc["attackSpeed"].GetFloat();
}