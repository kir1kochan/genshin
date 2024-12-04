#ifndef WEAPON_H
#define WEAPON_H

#include "../Equipment.h"
#include "Classes/Common/Entities/Player/Player.h"

// 武器类，继承自装备类
class Weapon : public Equipment {
private:
    float attackRange;  // 武器攻击范围
    float attackSpeed;  // 武器攻击频率（每秒攻击次数）

public:
    // 构造函数
    Weapon(int id, const std::string& name, int attackPower, float attackRange, float attackSpeed);

    // 获取攻击范围
    float getAttackRange() const;

    // 获取攻击频率
    float getAttackSpeed() const;

    // 重写打印信息函数
    void printInfo() const override;
};

#endif // WEAPON_H