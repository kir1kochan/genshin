#include "AttackSkill.h"
#include "cocos2d.h"

AttackSkill::AttackSkill(int id, const std::string& name, float cooldown, float attackPower, float range, Element element)
    : Skill(id, name, cooldown), attackPower(attackPower), range(range),element(element){}

// ��ȡ������
float AttackSkill::getAttackPower() const {
    return attackPower;
}

// ��ȡ��Χ
float AttackSkill::getRange() const {
    return range;
}

// �����
void AttackSkill::activate(Player* user, Enemy& target) {
    // ���Ŀ���Ƿ��ڷ�Χ��
    float distance = user->getPosition().distance(target.getPosition());
    if (distance > range) {
        CCLOG("AttackSkill: Target out of range for %s.", name.c_str());
        return;
    }

    // ��Ŀ������˺�
    user->attackTarget(target, attackPower,element);
    CCLOG("%s dealt %.2f damage to target.", name.c_str(), attackPower);
}