#include "Weapon.h"

// ���캯��
Weapon::Weapon(int id, const std::string& name, int attackPower, float attackRange, float attackSpeed)
    : Equipment(id, name, attackPower), attackRange(attackRange), attackSpeed(attackSpeed) {}

// ��ȡ������Χ
float Weapon::getAttackRange() const {
    return attackRange;
}

// ��ȡ����Ƶ��
float Weapon::getAttackSpeed() const {
    return attackSpeed;
}

// ��д��ӡ��Ϣ����
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
    CCLOG("Attack Range: %.2f", attackRange);
    CCLOG("Attack Speed: %.2f", attackSpeed);
}