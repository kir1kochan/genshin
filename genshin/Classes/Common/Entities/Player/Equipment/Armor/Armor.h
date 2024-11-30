#ifndef ARMOR_H
#define ARMOR_H

#include "Equipment.h"

// 护甲类，继承自装备类
class Armor : public Equipment {
public:
    // 构造函数
    Armor((int id, const std::string& name, int defense);

    // 重写打印信息函数
    void printInfo() const override;
};

#endif // ARMOR_H
