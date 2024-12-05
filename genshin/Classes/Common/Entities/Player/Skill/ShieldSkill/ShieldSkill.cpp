#include "ShieldSkill.h"
#include "ShieldSkill.h"
#include "cocos2d.h"
#include "../../Player.h"



ShieldSkill::ShieldSkill(int id, const std::string& name, float cooldown, float shieldValue, float shieldTime) 
    : Skill(id, name, cooldown), shieldValue(shieldValue), shieldTime(shieldTime){}

// ����ܼ���
void ShieldSkill::activate(Player* user, Enemy& target) {
    user->setShield(shieldValue,shieldTime);

    CCLOG("%s provides %.2f shield .", name.c_str(), shieldValue);
}

void ShieldSkill::loadFromJson(const std::string& jsonString) {
    Skill::loadFromJson(jsonString);  // ���ػ��������

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for ShieldSkill");
        return;
    }

    // ���� ShieldSkill ���е�����
    if (doc.HasMember("shieldValue") && doc["shieldValue"].IsFloat()) {
        shieldValue = doc["shieldValue"].GetFloat();
    }

    if (doc.HasMember("shieldTime") && doc["shieldTime"].IsFloat()) {
        shieldTime = doc["shieldTime"].GetFloat();
    }
}