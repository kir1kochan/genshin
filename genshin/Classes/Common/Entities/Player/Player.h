#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"

class Player : public Entities {
private:
    int experience;  // 玩家经验值
    int level;       // 玩家等级

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
};

#endif // PLAYER_H
