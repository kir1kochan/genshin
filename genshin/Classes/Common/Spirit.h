#ifndef SPIRIT_H
#define SPIRIT_H

#include "cocos2d.h"
#include "Element.h"

class Spirit : public cocos2d::Node {
protected:
    float health;          // 血量
    float maxHealth;       // 最大血量
    Element element;       // 元素属性

public:
    Spirit(float health, Element element)
        : health(health), maxHealth(health), element(element) {}
    
    // 用于测试的默认构造函数
    Spirit() : health(100), maxHealth(100), element(Element::FIRE) {}

    virtual ~Spirit() {}

    // 获取当前血量
    float getHealth() const { 
        return health; 
    }

    // 获取最大血量
    float getMaxHealth() const { 
        return maxHealth; 
    }

    // 元素类型
    Element getElement() const { 
        return element; 
    }

    // 受到伤害
    virtual void takeDamage(float amount) {
        health -= amount;
        if (health < 0) health = 0;
    }

    // 恢复血量
    virtual void heal(float amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    // 基本攻击
    virtual void attack(Spirit& target) {
        target.takeDamage(10.0f);  // 简单的攻击逻辑
    }

    // 打印当前状态
    virtual void printStatus() {
        CCLOG("Health: %.2f/%.2f", health, maxHealth);
        CCLOG("Element: %d", static_cast<int>(element));
    }
};

#endif // SPIRIT_H
