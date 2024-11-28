#include "Weapon.h"

// 构造函数
Weapon::Weapon(const std::string& name, int attackPower)
    : Equipment(name, attackPower) {}

// 重写打印信息函数
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
}