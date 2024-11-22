#ifndef PLAYERSPIRIT_H
#define PLAYERSPIRIT_H

#include "../Spirit.h"

class PlayerSpirit : public Spirit {
private:
    int experience;  // 玩家经验值
    int level;       // 玩家等级

public:
    PlayerSpirit(float health, Element element)
        : Spirit(health, element), experience(0), level(1) {}

    // 用于测试的默认构造函数
    PlayerSpirit() : Spirit(100, Element::WATER){}

    // 升级
    void levelUp() {
        level++;
        maxHealth += 20.0f;  // 每次升级增加最大血量
        health = maxHealth;   // 血量恢复
        CCLOG("Level up! Now level %d", level);
    }

    // 获取当前等级
    int getLevel() const { return level; }

    // 经验增加
    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {  // 假设100经验值升级
            experience = 0;
            levelUp();
        }
    }

    // 攻击玩家敌人时根据元素相克
    void attack(Spirit& target) override {
        if (element == Element::FIRE && target.getElement() == Element::EARTH) {
            target.takeDamage(20.0f);  // 火克土
        }
        else {
            target.takeDamage(10.0f);   // 默认伤害
        }
    }

    // 玩家独特的技能
    void castSkill() {
        CCLOG("Player casts a special skill!");
    }

    // 打印玩家状态
    void printStatus() override {
        Spirit::printStatus();
        CCLOG("Experience: %d", experience);
        CCLOG("Level: %d", level);
    }
};

#endif // PLAYERSPIRIT_H

