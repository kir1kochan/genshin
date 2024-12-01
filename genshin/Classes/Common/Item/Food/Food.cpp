#include "Food.h"
#include"Player.h"

Food::Food(int id, const std::string& name, float healAmount)
    : Item(id, name), healAmount(healAmount) {}

float Food::getHealAmount() const {
    return healAmount;
}

void Food::printInfo() const {
    Item::printInfo();
    CCLOG("Food Restore Amount: %.2f", healAmount);
}

std::string Food::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 基础数据
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("healAmount", healAmount, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

void Food::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing Food JSON");
        return;
    }

    Item::loadFromJson(jsonString);  // 调用父类的加载函数
    if (doc.HasMember("healAmount")) healAmount = doc["healAmount"].GetFloat();
}

void Food::use(Player* user)
{
    user->heal(healAmount);
    CCLOG("%s healed %.2f health.", name.c_str(), healAmount);
}
