#include "Backpack.h"
#include "../Item/Item.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>

// ���캯������ʼ����������������Ʒָ��������ʼ��Ϊ0
Backpack::Backpack(const std::vector<std::shared_ptr<Item>>& itemPointers): coins(0) {
    for (auto& item : itemPointers) {
        items[item] = 0;  // ��ÿ����Ʒָ���������ʼ��Ϊ0
        idToItemMap[item->getId()] = item;  // Ϊÿ����Ʒ����ID����Ʒָ���ӳ��
    }
}

Backpack::Backpack() : coins(0) {
    loadFromFile("JSON/Backpack.json");
}

// ͨ����Ʒָ�������Ʒ
void Backpack::addItem(const std::shared_ptr<Item>& item, int count) {
    if (items.find(item) != items.end()) {
        items[item] += count;  // ��������
    }
    else {
        items[item] = 0;  
        idToItemMap[item->getId()] = item;  
        CCLOG("Item not initialized in backpack: %s", item->getName().c_str());
    }
}

// ͨ����ƷID�����Ʒ
void Backpack::addItem(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        auto item = idToItemMap[itemId];
        addItem(item, count);  // ����ͨ����Ʒָ�������Ʒ�ĺ���
        if (itemId / 10000 == 3) {
            sendFoodBroadcast();
        }
    }
    else {
        CCLOG("Item with ID %d not found in backpack", itemId);
    }
}

// ͨ����Ʒָ���Ƴ���Ʒ
void Backpack::removeItem(const std::shared_ptr<Item>& item, int count) {
    if (items.find(item) != items.end()) {
        if (items[item] >= count) {
            items[item] -= count;  // ��������
        }
        else {
            // ������Լ��ٵ���������������������ӡ������Ϣ
            CCLOG("Error: Attempted to remove more items than available for %s", item->getName().c_str());
        }
    }
    else {
        CCLOG("Item not found in backpack: %s", item->getName().c_str());
    }
}

// ͨ����ƷID�Ƴ���Ʒ
void Backpack::removeItem(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        auto item = idToItemMap[itemId];
        removeItem(item, count);  // ����ͨ����Ʒָ���Ƴ���Ʒ�ĺ���
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
        CCLOG("Quantity: %d", pair.second);  // ��ӡ����
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
            // �� itemJson ת��Ϊ�ַ���
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            itemJson.Accept(writer);  // ������ itemJson ת�����ַ���
            std::string jsonStr = buffer.GetString();  // ��ȡת������ַ���

            // ʹ��ת������ַ�����������Ʒ
            auto item = createItemById(itemId, jsonStr);

            if (item) {
                addItem(item, quantity);  // ͨ����Ʒָ�������Ʒ������
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
    int itemType = id / 100000;   // ��Ʒ���ͣ�ID�ĵ�һ����
    int subType = id % 1000 / 100; // �����ͣ�ID�ĵڶ�����

    std::shared_ptr<Item> item = nullptr;
    switch (itemType) {
    case 1: // װ����
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
    case 2: // ҩ����
        item = std::make_shared<Potion>(id, "Health Potion", 50);
        break;
    case 3: // ʳ����
        item = std::make_shared<Food>(id, "Apple", 20);
        break;
    default:
        CCLOG("Error: Unsupported item type %d for ID %d", itemType, id);
        break;
    }

    // �����������Ʒ���󣬼��� JSON ����
    if (item) {
        item->loadFromJson(jsonString);
    }

    return item; 
}

// ������ƷID��ȡ��Ʒ����
int Backpack::getItemCountById(int itemId) const {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        auto item = idToItemMap.at(itemId);
        auto it = items.find(item);
        if (it != items.end()) {
            return it->second;  // ���ظ���Ʒ������
        }
    }
    return 0;  // ���û���ҵ�����ƷID������0
}

// ���͹㲥����ʶ����ʳ������б䶯
void Backpack::sendFoodBroadcast() {
    CCLOG("Backpack updated.");
    cocos2d::EventCustom event("BACKPACK_UPDATED");
    event.setUserData(this);  // ������ָ�봫�ݸ�������
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);  // �����¼�
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
                addItem(itemId, quantity);  // ���ش浵�е���Ʒ����
            }
        }
    }
    if (doc.HasMember("coins") && doc["coins"].IsInt()) {
        coins = doc["coins"].GetInt();
    }
}
