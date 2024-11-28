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