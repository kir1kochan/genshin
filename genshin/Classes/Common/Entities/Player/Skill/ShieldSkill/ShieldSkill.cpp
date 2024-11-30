#include "ShieldSkill.h"
#include "cocos2d.h"

ShieldSkill::ShieldSkill(int id, const std::string& name, float cooldown, float shieldValue)
    : Skill(id, name, cooldown), shieldValue(shieldValue) {}



// 激活护盾技能
void ShieldSkill::activate(Entities* user, Entities& target) {
    user->setShield(shieldValue);

    CCLOG("%s provides %.2f shield .", name.c_str(), shieldValue);
}