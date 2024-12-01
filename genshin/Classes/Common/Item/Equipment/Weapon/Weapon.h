#ifndef WEAPON_H
#define WEAPON_H

#include "Equipment.h"

// 武器类，继承自装备类

//可以进一步完善，按照攻击距离划分
class Weapon : public Equipment {
public:
    // 构造函数
    Weapon(int id, const std::string& name, int attackPower);

    // 重写打印信息函数
    void printInfo() const override;
};

#endif // WEAPON_H