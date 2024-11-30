#include "Accessory.h"

// 构造函数
Accessory::Accessory(int id, const std::string& name, int power)
    : Equipment(id, name, power) {}

// 重写打印信息函数
void Accessory::printInfo() const {
    CCLOG("Accessory Name: %s", getName().c_str());
    CCLOG("Power: %d", getPower());
}