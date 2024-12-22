#include "Backpack.h"
#include "../Item/Item.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>

// 构造函数，初始化背包，将所有物品指针数量初始化为0
Backpack::Backpack(const std::vector<std::shared_ptr<Item>>& itemPointers): coins(0) {
    for (auto& item : itemPointers) {
        items[item] = 0;  // 将每个物品指针的数量初始化为0
        idToItemMap[item->getId()] = item;  // 为每个物品创建ID到物品指针的映射
    }
}

Backpack::Backpack() : coins(0) {
    loadFromFile("JSON/Backpack.json");
}

// 通过物品指针添加物品
void Backpack::addItem(const std::shared_ptr<Item>& item, int count) {
    if (items.find(item) != items.end()) {
        items[item] += count;  // 增加数量
    }
    else {
        items[item] = 0;  
        idToItemMap[item->getId()] = item;  
        CCLOG("Item not initialized in backpack: %s", item->getName().c_str());
    }
}

// 通过物品ID添加物品
void Backpack::addItem(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        auto item = idToItemMap[itemId];
        addItem(item, count);  // 调用通过物品指针添加物品的函数
        if (itemId / 10000 == 3) {
            sendFoodBroadcast();
        }
    }
    else {
        CCLOG("Item with ID %d not found in backpack", itemId);
    }
}

// 通过物品指针移除物品
void Backpack::removeItem(const std::shared_ptr<Item>& item, int count) {
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
        auto item = idToItemMap[itemId];
        removeItem(item, count);  // 调用通过物品指针移除物品的函数
        if (itemId / 10000 == 3) {
            sendFoodBroadcast();
        }
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

    doc.AddMember("coins", coins, allocator);


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
            auto item = createItemById(itemId, jsonStr);

            if (item) {
                addItem(item, quantity);  // 通过物品指针添加物品到背包
            }
        }
    }
    if (doc.HasMember("coins") && doc["coins"].IsInt()) {
        coins = doc["coins"].GetInt();
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

std::shared_ptr<Item> Backpack::createItemById(int id, const std::string& jsonString) {
    int itemType = id / 100000;   // 物品类型：ID的第一部分
    int subType = id % 1000 / 100; // 子类型：ID的第二部分

    std::shared_ptr<Item> item = nullptr;
    switch (itemType) {
    case 1: // 装备类
        switch (subType) {
        case 1: {
            item = std::make_shared<Weapon>(id, "Weapon Name", 5.0f, 10.0f, 1.0f);
            break;
        }
        case 2: {
            item = std::make_shared<Armor>(id, "Armor Name", 5);
            break;
        }
        case 3: {
            item = std::make_shared<Accessory>(id, "Accessory Name", 3,Element::FIRE);
            break;
        }
        default:
            CCLOG("Error: Unsupported equipment subtype %d for ID %d", subType, id);
            break;
        }
        break;
    case 2: // 药剂类
        item = std::make_shared<Potion>(id, "Health Potion", 50);
        break;
    case 3: // 食物类
        item = std::make_shared<Food>(id, "Apple", 20);
        break;
    default:
        CCLOG("Error: Unsupported item type %d for ID %d", itemType, id);
        break;
    }

    // 如果创建了物品对象，加载 JSON 数据
    if (item) {
        item->loadFromJson(jsonString);
    }

    return item; 
}

// 根据物品ID获取物品数量
int Backpack::getItemCountById(int itemId) const {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        auto item = idToItemMap.at(itemId);
        auto it = items.find(item);
        if (it != items.end()) {
            return it->second;  // 返回该物品的数量
        }
    }
    return 0;  // 如果没有找到该物品ID，返回0
}

// 发送广播，标识背包食物相关有变动
void Backpack::sendFoodBroadcast() {
    CCLOG("Backpack updated.");
    cocos2d::EventCustom event("BACKPACK_UPDATED");
    event.setUserData(this);  // 将背包指针传递给监听者
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);  // 发送事件
}

int Backpack::getCoins() const {
    return coins;
}

void Backpack::addCoins(int amount) {
    if (amount > 0) {
        coins += amount;
    }
}

bool Backpack::useCoins(int amount) {
    if (coins >= amount && amount > 0) {
        coins -= amount;
        return true;
    }
    return false;
}

const std::unordered_map<std::shared_ptr<Item>, int>& Backpack::getItems() const{
    return items;
}

std::shared_ptr<Item> Backpack::findItemById(int id) {
    return idToItemMap[id];
}

void Backpack::loadFromJson(const std::string& jsonString, int i) {
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
            if (quantity) {
                addItem(itemId, quantity);  // 加载存档中的物品数量
            }
        }
    }
    if (doc.HasMember("coins") && doc["coins"].IsInt()) {
        coins = doc["coins"].GetInt();
    }
}
