#include "Equipment.h"

// ���캯��
Equipment::Equipment(const std::string& name, int power)
    : name(name), power(power) {}

// ��ȡװ������
const std::string& Equipment::getName() const {
    return name;
}

// ��ȡװ�������ԣ����������������
int Equipment::getPower() const {
    return power;
}

// ��ӡװ����Ϣ�����⺯��������������д��
void Equipment::printInfo() const {
    CCLOG("Equipment Name: %s", name.c_str());
    CCLOG("Power: %d", power);
}


// ��װ���������л�Ϊ JSON �ַ���
std::string Equipment::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // ��װ���ĳ�Ա�������л�Ϊ JSON ����
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("power", power, allocator);

    // ת��Ϊ JSON �ַ���
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// �� JSON �ַ�������װ������
void Equipment::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // �� JSON �м���װ������
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("power")) power = doc["power"].GetInt();
}

// ����Ա�������л�Ϊ JSON ��ʽ�������浽����
void Entities::saveToFile(const std::string& filePath) const {
    std::string jsonString = saveToJson(); // ���� saveToJson ��ȡ JSON �ַ���
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonString;
        file.close();
        CCLOG("Entity saved to file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

// �ӱ��ض�ȡ JSON �ļ�����ȡ��Ա��������
void Entities::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf(); // ��ȡ�����ļ����ݵ� buffer
        file.close();
        std::string jsonString = buffer.str();
        loadFromJson(jsonString); // ���� loadFromJson ���� JSON ����
        CCLOG("Entity loaded from file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}