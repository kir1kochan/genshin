#include "Potion.h"

// 构造函数
Potion::Potion(int id, const std::string& name, float effectValue)
    : Item(id, name), effectValue(effectValue) {}

// 使用药剂的效果
// 尽量可以不与食物类重复，不只是回复血量，比如 一段时间内提高攻速、一段时间内有有吸血效果 、一段时间内加快移动速度等
void Potion::use(Player& player) const {
   
}

// 重写打印信息函数
void Potion::printInfo() const {
    CCLOG("Potion Name: %s", getName().c_str());
    CCLOG("Effect Value: %.2f", effectValue);
}

void Potion::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing Potion JSON");
        return;
    }

    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("effectValue")) effectValue = doc["effectValue"].GetFloat();
}

std::string Potion::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 添加成员变量到 JSON
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("effectValue", effectValue, allocator);

    // 转换为字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}