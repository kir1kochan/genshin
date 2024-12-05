#include "HealSkill.h"
#include "cocos2d.h"
#include "../../Player.h"

HealSkill::HealSkill(int id,const std::string& name, float cooldown, float healAmount)
    : Skill(id, name, cooldown), healAmount(healAmount) {}
HealSkill::HealSkill()
    : Skill(id, name, cooldown), healAmount(healAmount) {}
// �������Ƽ���
void HealSkill::activate(Player* user, Enemy& target) {
    user->heal(healAmount);
    CCLOG("%s healed %.2f health.", name.c_str(), healAmount);
}

void HealSkill::loadFromJson(const std::string& jsonString) {
    Skill::loadFromJson(jsonString);  // ���ػ��������

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for HealSkill");
        return;
    }

    // ���� HealSkill ���е�����
    if (doc.HasMember("healAmount") && doc["healAmount"].IsFloat()) {
        healAmount = doc["healAmount"].GetFloat();
    }
}