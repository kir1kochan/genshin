#include "Weapon.h"

// 构造函数
Weapon::Weapon(int id, const std::string& name, int attackPower, float attackRange, float attackSpeed)
    : Equipment(id, name, attackPower), attackRange(attackRange), attackSpeed(attackSpeed) {}

// 获取攻击范围
float Weapon::getAttackRange() const {
    return attackRange;
}

// 获取攻击频率
float Weapon::getAttackSpeed() const {
    return attackSpeed;
}

// 重写打印信息函数
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
    CCLOG("Attack Range: %.2f", attackRange);
    CCLOG("Attack Speed: %.2f", attackSpeed);
}