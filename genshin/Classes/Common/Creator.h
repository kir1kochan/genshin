#ifndef CREATOR_H
#define CREATOR_H

#include "cocos2d.h"
#include "Element.h"

class Creator : public cocos2d::Node {
protected:
    float health;          // 当前血量
    float maxHealth;       // 最大血量
    Element element;       // 元素属性

public:
    // 构造函数
    Creator(float health, Element element);

    // 默认构造函数
    Creator();

    // 虚析构函数
    virtual ~Creator();

    // 获取当前血量
    float getHealth() const;

    // 获取最大血量
    float getMaxHealth() const;

    // 获取元素类型
    Element getElement() const;

    // 受到伤害
    virtual void takeDamage(float amount);

    // 恢复血量
    virtual void heal(float amount);

    // 攻击目标
    virtual void attack(Creator& target);

    // 打印当前状态
    virtual void printStatus();
};

#endif // CREATOR_H
