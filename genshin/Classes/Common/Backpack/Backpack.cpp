#include "Backpack.h"
#include "Item.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>


// ���캯������ʼ����������������Ʒָ��������ʼ��Ϊ0
Backpack::Backpack(const std::vector<Item*>& itemPointers) {
    for (Item* item : itemPointers) {
        items[item] = 0;  // ��ÿ����Ʒָ���������ʼ��Ϊ0
        idToItemMap[item->getId()] = item;  // Ϊÿ����Ʒ����ID����Ʒָ���ӳ��
    }
}

// ͨ����Ʒָ�������Ʒ
void Backpack::addItem(Item* item, int count) {
    if (items.find(item) != items.end()) {
        items[item] += count;  // ��������
    }
    else {
        CCLOG("Item not initialized in backpack: %s", item->getName().c_str());
    }
}

// ͨ����ƷID�����Ʒ
void Backpack::addItemById(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        Item* item = idToItemMap[itemId];
        addItem(item, count);  // ����ͨ����Ʒָ�������Ʒ�ĺ���
    }
    else {
        CCLOG("Item with ID %d not found in backpack", itemId);
    }
}

// ͨ����Ʒָ���Ƴ���Ʒ
void Backpack::removeItem(Item* item, int count) {
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
void Backpack::removeItemById(int itemId, int count) {
    if (idToItemMap.find(itemId) != idToItemMap.end()) {
        Item* item = idToItemMap[itemId];
        removeItem(item, count);  // ����ͨ����Ʒָ���Ƴ���Ʒ�ĺ���
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
            Item* item = Item::createItemById(itemId, jsonStr);

            if (item) {
                addItem(item, quantity);  // ͨ����Ʒָ�������Ʒ������
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