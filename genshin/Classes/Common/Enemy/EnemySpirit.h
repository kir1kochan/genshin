#ifndef ENEMYSPIRIT_H
#define ENEMYSPIRIT_H

#include "../Spirit.h"

class EnemySpirit : public Spirit {
private:
    int aggressionLevel;  // 敌人的攻击性等级（决定敌人的攻击频率或强度）

public:
    EnemySpirit(float health, Element element, int aggressionLevel)
        : Spirit(health, element), aggressionLevel(aggressionLevel) {}

    // 用于测试的默认构造函数
    EnemySpirit() : Spirit(100, Element::EARTH) {}

    int getAttack() const { return aggressionLevel; }

    // 攻击敌人
    void attack(Spirit& target) override {
        if (aggressionLevel > 5) {
            target.takeDamage(15.0f);  // 高攻击性
        }
        else {
            target.takeDamage(5.0f);   // 低攻击性
        }
    }

    // 敌人AI行为
    void aiBehavior() {
        if (health < maxHealth / 2) {
            CCLOG("Enemy retreats!");
        }
        else {
            CCLOG("Enemy attacks with high aggression!");
        }
    }

    // 打印敌人状态
    void printStatus() override {
        Spirit::printStatus();
        CCLOG("Aggression Level: %d", aggressionLevel);
    }
};

#endif // ENEMYSPIRIT_H

