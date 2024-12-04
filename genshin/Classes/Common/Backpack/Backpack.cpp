#include "Backpack.h"
#include "../Item/Item.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>


// 构造函数，初始化背包，将所有物品指针数量初始化为0
Backpack::Backpack(const std::vector<Item*> itemPointers) {
    for (auto item : itemPointers) {
        items[item] = 0;  // 将每个物品指针的数量初始化为0
        idToItemMap[item->getId()] = item;  // 为每个物品创建ID到物品指针的映射
    }
}

Backpack::Backpack(){
}

// 通过物品指针添加物品
void Backpack::addItem(Item* item, int count) {
    if (items.find(item) != items.end()) {
        items[item] += count;  // 增加数量
    }
    else {
        CCLOG("Item not initialized in backpack: %s", item->getName().c_str());
    }
}

// 通过物品ID添加物品
void Backpack::addItem(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        Item* item = idToItemMap[itemId];
        addItem(item, count);  // 调用通过物品指针添加物品的函数
    }
    else {
        CCLOG("Item with ID %d not found in backpack", itemId);
    }
}

// 通过物品指针移除物品
void Backpack::removeItem(Item* item, int count) {
    if (items.find(item) != items.end()) {
        if (items[item] >= count) {
            items[item] -= count;  // 减少数量
        }
        else {
            // 如果尝试减少的数量大于现有数量，打印错误信息
            CCLOG("Error: Attempted to remove more items than available for %s", item->getName().c_str());
        }
    }
    else {
        CCLOG("Item not found in backpack: %s", item->getName().c_str());
    }
}

// 通过物品ID移除物品
void Backpack::removeItem(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        Item* item = idToItemMap[itemId];
        removeItem(item, count);  // 调用通过物品指针移除物品的函数
    }
    else {
        CCLOG("Item with ID %d not found in backpack", itemId);
    }
}

void Backpack::printInfo() const {
    for (const auto& pair : items) {
        pair.first->printInfo();
        CCLOG("Quantity: %d", pair.second);  // 打印数量
    }
}

std::string Backpack::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    rapidjson::Value itemArray(rapidjson::kArrayType);
    for (const auto& pair : items) {
        rapidjson::Value itemObj(rapidjson::kObjectType);
        itemObj.AddMember("id", pair.first->getId(), allocator);
        itemObj.AddMember("quantity", pair.second, allocator);
        itemArray.PushBack(itemObj, allocator);
    }
    doc.AddMember("items", itemArray, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

void Backpack::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    if (doc.HasMember("items") && doc["items"].IsArray()) {
        for (const auto& itemJson : doc["items"].GetArray()) {
            int itemId = itemJson["id"].GetInt();
            int quantity = itemJson["quantity"].GetInt();
            // 将 itemJson 转换为字符串
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            itemJson.Accept(writer);  // 这里会把 itemJson 转换成字符串
            std::string jsonStr = buffer.GetString();  // 获取转换后的字符串

            // 使用转换后的字符串来创建物品
            Item* item = createItemById(itemId, jsonStr);

            if (item) {
                addItem(item, quantity);  // 通过物品指针添加物品到背包
            }
        }
    }
}

void Backpack::saveToFile(const std::string& filePath) const {
    std::string jsonString = saveToJson();
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonString;
        file.close();
        CCLOG("Backpack saved to file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

void Backpack::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        std::string jsonString = buffer.str();
        loadFromJson(jsonString);
        CCLOG("Backpack loaded from file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

Item* Backpack::createItemById(int id, const std::string& jsonString) {
    int itemType = id / 10000;   // 物品类型：ID的第一部分
    int subType = (id / 100) % 100; // 子类型：ID的第二部分

    // 根据物品类型和子类型和字符串创建不同的物品对象
    Item* item = nullptr;  // 在这里声明一个指针变量，用于存储物品对象

    switch (itemType) {
    case 1: // 装备类
        switch (subType) {
        case 101: {
            item = new Weapon(id, "Weapon Name", 5.0f, 10.0f, 1.0f);
            item->loadFromJson(jsonString);
            return item;
        }
        case 102: {
            item = new Armor(id, "Armor Name", 5);
            item->loadFromJson(jsonString);
            return item;
        }
        case 103: {
            item = new Accessory(id, "Accessory Name", 3);
            item->loadFromJson(jsonString);
            return item;
        }
        default: break;
        }
        break;
    case 2: // 药剂类
        item = new Potion(id, "Health Potion", 50);
        item->loadFromJson(jsonString);
        return item;
    case 3: // 食物类
        item = new Food(id, "Apple", 20);
        item->loadFromJson(jsonString);
        return item;
    default:
        break;
    }
    

    return nullptr;  // 返回空指针，表示未找到有效类型
}