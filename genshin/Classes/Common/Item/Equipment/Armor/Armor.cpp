#include "Armor.h"

// 构造函数
Armor::Armor(int id, const std::string& name, int power)
    : Equipment(id, name, power) {}

// 重写打印信息函数
void Armor::printInfo() const {
    CCLOG("Armor Name: %s", getName().c_str());
    CCLOG("Defense: %d", getPower());
}