#include "AttackSkill.h"
#include "cocos2d.h"

AttackSkill::AttackSkill(const std::string& name, float cooldown, float attackPower, float range)
    : Skill(name, cooldown), attackPower(attackPower), range(range) {}

// ��ȡ������
float AttackSkill::getAttackPower() const {
    return attackPower;
}

// ��ȡ��Χ
float AttackSkill::getRange() const {
    return range;
}

// �����
void AttackSkill::activate(Entities* user, Entities& target) {
    // ���Ŀ���Ƿ��ڷ�Χ��
    float distance = user->getPosition().distance(target.getPosition());
    if (distance > range) {
        CCLOG("AttackSkill: Target out of range for %s.", name.c_str());
        return;
    }

    // ��Ŀ������˺�
    target.takeDamage(attackPower);
    CCLOG("%s dealt %.2f damage to target.", name.c_str(), attackPower);
}