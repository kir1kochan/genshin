#include "HealSkill.h"
#include "cocos2d.h"

HealSkill::HealSkill(const std::string& name, float cooldown, float healAmount)
    : Skill(name, cooldown), healAmount(healAmount) {}

// �������Ƽ���
void HealSkill::activate(Entities* user, Entities& target) {
    user->heal(healAmount);
    CCLOG("%s healed %.2f health.", name.c_str(), healAmount);
}