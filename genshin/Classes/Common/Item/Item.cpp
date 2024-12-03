#include "Item.h"

Item::Item(int id, const std::string& name) : id(id), name(name) {}

int Item::getId() const {
    return id;
}

void Item::printInfo() const
{
    CCLOG("Item Name: %s", name.c_str());
    CCLOG("ID: %d", id);
}

const std::string& Item::getName() const {
    return name;
}

std::string Item::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

void Item::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
}

// 从文件加载物品数据
void Item::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        loadFromJson(buffer.str());  // 从字符串加载
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

// 将物品数据保存到文件
void Item::saveToFile(const std::string& filePath) const {
    std::string jsonString = saveToJson();
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonString;
        file.close();
        CCLOG("Item saved to file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

Item* Item::createItemById(int id, const std::string& jsonString) {
    int itemType = id / 10000;   // 物品类型：ID的第一部分
    int subType = (id / 100) % 100; // 子类型：ID的第二部分

    // 根据物品类型和子类型和字符串创建不同的物品对象
    switch (itemType) {
    case 1: // 装备类
        switch (subType) {
        case 101: 
            Weapon* weapon = new Weapon(id, "Weapon Name", 5.0f,10.0f,1.0f);
            weapon->loadFromJson(jsonString);
            return weapon;
        case 102: 
            Armor* armor = new Armor(id, "Armor Name", 5);
            armor->loadFromJson(jsonString);
            return armor;
        case 103: 
            Accessory* accessory =  new Accessory(id, "Accessory Name", 3);
            accessory->loadFromJson(jsonString);
            return accessory;
        default: break;
        }
        break;
    case 2: // 药剂类
        Potion* potion = new Potion(id, "Health Potion", 50);
        potion->loadFromJson(jsonString);
        return potion;
    case 3: // 食物类
        Food* food = new Food(id, "Apple", 20);
        food->loadFromJson(jsonString);
        return food;
    default:
        break;
    }

    return nullptr;  // 返回空指针，表示未找到有效类型
}