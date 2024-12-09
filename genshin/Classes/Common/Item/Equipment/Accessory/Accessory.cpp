#include "Accessory.h"


// 构造函数
Accessory::Accessory(int id, const std::string& name, int power,Element element)
    : Equipment(id, name, power),element(element) {}

// 保存为 JSON 字符串
std::string Accessory::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 序列化父类装备属性
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("power", power, allocator);

    // 序列化元素属性
    doc.AddMember("element", static_cast<int>(element), allocator);

    // 转换为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串加载数据
void Accessory::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Accessory");
        return;
    }

    // 加载父类装备数据
    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("power")) power = doc["power"].GetInt();

    // 加载元素属性
    if (doc.HasMember("element")) {
        element = static_cast<Element>(doc["element"].GetInt());
    }
}

// 打印饰品信息
void Accessory::printInfo() const {
    CCLOG("Accessory ID: %d", id);
    CCLOG("Accessory Name: %s", name.c_str());
    CCLOG("Power: %d", power);
    CCLOG("Element Type: %d", static_cast<int>(element));  // 打印元素类型
}

Element Accessory::getElement()
{
    return element;
}
