#include "Player.h"
#include "cocos2d.h"

Player::Player(float health, Element element)
    : Creator(health, element), experience(0), level(1) {}

Player::Player() : Creator(100, Element::WATER), experience(0), level(1) {}

void Player::levelUp() {
    level++;
    maxHealth += 20.0f;  // 每次升级增加最大血量
    health = maxHealth;   // 血量恢复
    CCLOG("Level up! Now level %d", level);
}

int Player::getLevel() const {
    return level;
}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {  // 假设100经验值升级
        experience = 0;
        levelUp();
    }
}

void Player::attack(Creator& target) {
    if (element == Element::FIRE && target.getElement() == Element::EARTH) {
        target.takeDamage(20.0f);  // 火克土
    }
    else {
        target.takeDamage(10.0f);   // 默认伤害
    }
}

void Player::castSkill() {
    CCLOG("Player casts a special skill!");
}

void Player::printStatus() {
    Creator::printStatus();
    CCLOG("Experience: %d", experience);
    CCLOG("Level: %d", level);
}
