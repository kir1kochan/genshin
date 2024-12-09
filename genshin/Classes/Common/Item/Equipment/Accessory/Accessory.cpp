#include "Accessory.h"


// ���캯��
Accessory::Accessory(int id, const std::string& name, int power,Element element)
    : Equipment(id, name, power),element(element) {}

// ����Ϊ JSON �ַ���
std::string Accessory::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // ���л�����װ������
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("power", power, allocator);

    // ���л�Ԫ������
    doc.AddMember("element", static_cast<int>(element), allocator);

    // ת��Ϊ JSON �ַ���
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// �� JSON �ַ�����������
void Accessory::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Accessory");
        return;
    }

    // ���ظ���װ������
    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("power")) power = doc["power"].GetInt();

    // ����Ԫ������
    if (doc.HasMember("element")) {
        element = static_cast<Element>(doc["element"].GetInt());
    }
}

// ��ӡ��Ʒ��Ϣ
void Accessory::printInfo() const {
    CCLOG("Accessory ID: %d", id);
    CCLOG("Accessory Name: %s", name.c_str());
    CCLOG("Power: %d", power);
    CCLOG("Element Type: %d", static_cast<int>(element));  // ��ӡԪ������
}

Element Accessory::getElement()
{
    return element;
}
