#include "Potion.h"

// ���캯��
Potion::Potion(int id, const std::string& name, float effectValue)
    : Item(id, name), effectValue(effectValue) {}

// ʹ��ҩ����Ч��
// �������Բ���ʳ�����ظ�����ֻ�ǻظ�Ѫ�������� һ��ʱ������߹��١�һ��ʱ����������ѪЧ�� ��һ��ʱ���ڼӿ��ƶ��ٶȵ�
void Potion::use(Player& player) const {
   
}

// ��д��ӡ��Ϣ����
void Potion::printInfo() const {
    CCLOG("Potion Name: %s", getName().c_str());
    CCLOG("Effect Value: %.2f", effectValue);
}

void Potion::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing Potion JSON");
        return;
    }

    if (doc.HasMember("id")) id = doc["id"].GetInt();
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("effectValue")) effectValue = doc["effectValue"].GetFloat();
}

std::string Potion::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // ��ӳ�Ա������ JSON
    doc.AddMember("id", id, allocator);
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("effectValue", effectValue, allocator);

    // ת��Ϊ�ַ���
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}