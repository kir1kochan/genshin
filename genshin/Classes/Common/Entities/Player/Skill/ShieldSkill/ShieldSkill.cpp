#include "ShieldSkill.h"
#include "cocos2d.h"

ShieldSkill::ShieldSkill(const std::string& name, float cooldown, float shieldValue)
    : Skill(name, cooldown), shieldValue(shieldValue) {}



// ����ܼ���
void ShieldSkill::activate(Entities* user, Entities& target) {
    user->setShield(shieldValue);

    CCLOG("%s provides %.2f shield .", name.c_str(), shieldValue);
}