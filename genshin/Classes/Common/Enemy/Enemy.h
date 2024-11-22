#ifndef ENEMY_H
#define ENEMY_H

#include "../Creator.h"

class Enemy : public Creator {
private:
    int aggressionLevel;  // 敌人的攻击性等级（决定敌人的攻击频率或强度）

public:
    Enemy(float health, Element element, int aggressionLevel);
    Enemy();  // 默认构造函数

    int getAggressionLevel() const;

    // 攻击敌人
    void attack(Creator& target) override;

    // 敌人AI行为
    void aiBehavior();

    // 打印敌人状态
    void printStatus() override;
};

#endif // ENEMY_H
