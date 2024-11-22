#include "Enemy.h"
#include "cocos2d.h"

Enemy::Enemy(float health, Element element, int aggressionLevel)
    : Creator(health, element), aggressionLevel(aggressionLevel) {}

Enemy::Enemy() : Creator(100, Element::EARTH), aggressionLevel(1) {}

int Enemy::getAggressionLevel() const {
    return aggressionLevel;
}

void Enemy::attack(Creator& target) {
    if (aggressionLevel > 5) {
        target.takeDamage(15.0f);  // �߹�����
    }
    else {
        target.takeDamage(5.0f);   // �͹�����
    }
}

void Enemy::aiBehavior() {
    if (health < maxHealth / 2) {
        CCLOG("Enemy retreats!");
    }
    else {
        CCLOG("Enemy attacks with high aggression!");
    }
}

void Enemy::printStatus() {
    Creator::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
}
