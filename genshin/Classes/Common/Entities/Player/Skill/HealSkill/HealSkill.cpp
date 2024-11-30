#include "HealSkill.h"
#include "cocos2d.h"

HealSkill::HealSkill(int id,const std::string& name, float cooldown, float healAmount)
    : Skill(id, name, cooldown), healAmount(healAmount) {}

// �������Ƽ���
void HealSkill::activate(Player* user, Enemy& target) {
    user->heal(healAmount);
    CCLOG("%s healed %.2f health.", name.c_str(), healAmount);
}