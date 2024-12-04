#include "ShieldSkill.h"
#include "cocos2d.h"
#include "../../Player.h"

ShieldSkill::ShieldSkill(int id, const std::string& name, float cooldown, float shieldTime)
    : Skill(id, name, cooldown), shieldValue(shieldValue), shieldTime(shieldTime){}



// 激活护盾技能
void ShieldSkill::activate(Player* user, Enemy& target) {
    user->setShield(shieldValue,shieldTime);

    CCLOG("%s provides %.2f shield .", name.c_str(), shieldValue);
}