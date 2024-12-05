#include "HealSkill.h"
#include "cocos2d.h"
#include "../../Player.h"

HealSkill::HealSkill(int id,const std::string& name, float cooldown, float healAmount)
    : Skill(id, name, cooldown), healAmount(healAmount) {}
HealSkill::HealSkill()
    : Skill(id, name, cooldown), healAmount(healAmount) {}
// 激活治疗技能
void HealSkill::activate(Player* user, Enemy& target) {
    user->heal(healAmount);
    CCLOG("%s healed %.2f health.", name.c_str(), healAmount);
}

void HealSkill::loadFromJson(const std::string& jsonString) {
    Skill::loadFromJson(jsonString);  // 加载基类的属性

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for HealSkill");
        return;
    }

    // 解析 HealSkill 特有的属性
    if (doc.HasMember("healAmount") && doc["healAmount"].IsFloat()) {
        healAmount = doc["healAmount"].GetFloat();
    }
}