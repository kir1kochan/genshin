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

// ���ļ�������Ʒ����
void Item::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        loadFromJson(buffer.str());  // ���ַ�������
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

// ����Ʒ���ݱ��浽�ļ�
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

Item* Item::createItemById(int id) {
    int itemType = id / 10000;   // ��Ʒ���ͣ�ID�ĵ�һ����
    int subType = (id / 100) % 100; // �����ͣ�ID�ĵڶ�����

    // ������Ʒ���ͺ������ʹ�����ͬ����Ʒ����
    switch (itemType) {
    case 1: // װ����
        switch (subType) {
        case 101: return new Weapon(id, "Weapon Name", 10); // ʾ��
        case 102: return new Armor(id, "Armor Name", 5);    // ʾ��
        case 103: return new Accessory(id, "Accessory Name", 3); // ʾ��
        default: std::cerr << "Unknown Equipment subtype\n"; break;
        }
        break;
    case 2: // ҩ����
        return new Potion(id, "Health Potion", 50);  // ʾ��
    case 3: // ʳ����
        return new Food(id, "Apple", 20); // ʾ��
    default:
        std::cerr << "Unknown item type\n";
        break;
    }

    return nullptr;  // ���ؿ�ָ�룬��ʾδ�ҵ���Ч����
}