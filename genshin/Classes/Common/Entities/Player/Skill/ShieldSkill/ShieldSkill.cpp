#include "ShieldSkill.h"
#include "ShieldSkill.h"
#include "cocos2d.h"
#include "../../Player.h"



ShieldSkill::ShieldSkill(int id, const std::string& name, float cooldown, float shieldValue, float shieldTime) 
    : Skill(id, name, cooldown), shieldValue(shieldValue), shieldTime(shieldTime){}

// 激活护盾技能
void ShieldSkill::activate(Player* user, Enemy& target) {
    user->setShield(shieldValue,shieldTime);

    CCLOG("%s provides %.2f shield .", name.c_str(), shieldValue);
}

void ShieldSkill::loadFromJson(const std::string& jsonString) {
    Skill::loadFromJson(jsonString);  // 加载基类的属性

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for ShieldSkill");
        return;
    }

    // 解析 ShieldSkill 特有的属性
    if (doc.HasMember("shieldValue") && doc["shieldValue"].IsFloat()) {
        shieldValue = doc["shieldValue"].GetFloat();
    }

    if (doc.HasMember("shieldTime") && doc["shieldTime"].IsFloat()) {
        shieldTime = doc["shieldTime"].GetFloat();
    }
}