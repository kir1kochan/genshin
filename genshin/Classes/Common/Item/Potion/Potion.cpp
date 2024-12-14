#include "Potion.h"
#include "Classes/Common/Entities/Player/Player.h"

// 构造函数
Potion::Potion(int id, const std::string& name, float healAmount)
    : Item(id, name), healAmount(healAmount) {}

void Potion::use(Player& user) const {
    user.regenerateStamina(healAmount);
}

// 获取恢复量
float Potion::getHealAmount() const {
    return healAmount;
}

// 打印药剂信息
void Potion::printInfo() const {
    CCLOG("Potion Name: %s", getName().c_str());
    CCLOG("Heal Amount: %.2f", healAmount);
}

// 从 JSON 加载药剂信息
void Potion::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing Potion JSON");
        return;
    }

    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("healAmount")) healAmount = doc["healAmount"].GetFloat();
}

// 保存药剂信息为 JSON 格式
std::string Potion::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 添加成员变量到 JSON
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("healAmount", healAmount, allocator);

    // 转换为字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}