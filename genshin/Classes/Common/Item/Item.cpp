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

