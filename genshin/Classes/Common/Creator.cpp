#include "Creator.h"

// 构造函数
Creator::Creator(float health, Element element)
    : health(health), maxHealth(health), element(element) {}

// 默认构造函数
Creator::Creator()
    : health(100), maxHealth(100), element(Element::FIRE) {}

// 虚析构函数
Creator::~Creator() {}

// 获取当前血量
float Creator::getHealth() const {
    return health;
}

// 获取最大血量
float Creator::getMaxHealth() const {
    return maxHealth;
}

// 获取元素类型
Element Creator::getElement() const {
    return element;
}

// 受到伤害
void Creator::takeDamage(float amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// 恢复血量
void Creator::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// 攻击目标
void Creator::attack(Creator& target) {
    target.takeDamage(10.0f); // 简单的攻击逻辑
}

// 打印当前状态
void Creator::printStatus() {
    CCLOG("Health: %.2f/%.2f", health, maxHealth);
    CCLOG("Element: %s", elementToString(element).c_str());
}
