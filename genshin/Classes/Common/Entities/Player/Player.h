#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"

#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"

class Player : public Entities {
private:
    int experience;  // 玩家经验值
    int level;       // 玩家等级

    // 装备（确保一个玩家只能佩戴一个装备）
    Weapon* weapon;           // 武器
    Armor* armor;             // 护甲
    Accessory* accessory;     // 饰品

public:
    Player(float health, Element element);
    Player();  // 默认构造函数

    // 升级
    void levelUp();

    // 获取当前等级
    int getLevel() const;

    // 经验增加
    void gainExperience(int exp);

    // 攻击玩家敌人时根据元素相克
    void attack(Entities& target) override;

    // 玩家独特的技能
    void castSkill();

    // 打印玩家状态
    void printStatus() override;

    // 装备管理
    void equipWeapon(Weapon* weapon);
    void equipArmor(Armor* armor);
    void equipAccessory(Accessory* accessory);

    // 获取装备
    Weapon* getWeapon() const;
    Armor* getArmor() const;
    Accessory* getAccessory() const;
};

#endif // PLAYER_H
