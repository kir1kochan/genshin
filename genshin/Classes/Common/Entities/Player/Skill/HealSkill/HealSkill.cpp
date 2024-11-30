#include "HealSkill.h"
#include "cocos2d.h"

HealSkill::HealSkill(const std::string& name, float cooldown, float healAmount)
    : Skill(name, cooldown), healAmount(healAmount) {}

// 激活治疗技能
void HealSkill::activate(Entities* user, Entities& target) {
    user->heal(healAmount);
    CCLOG("%s healed %.2f health.", name.c_str(), healAmount);
}