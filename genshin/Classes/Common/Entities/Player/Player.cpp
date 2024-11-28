#include "Player.h"
#include "cocos2d.h"

float Player::calculateElementalDamageModifier(Element attackerElement, Element targetElement) {
    // 属性相克关系
    if (attackerElement == Element::FIRE && targetElement == Element::EARTH ||
        attackerElement == Element::WATER && targetElement == Element::FIRE ||
        attackerElement == Element::EARTH && targetElement == Element::AIR ||
        attackerElement == Element::AIR && targetElement == Element::WATER ||
        ) {
        return 1.2f;  // 火克土\水克火\土克风\风克水
    }
    if (attackerElement == targetElement) {
        return 1.0f;  // 元素相同，正常伤害
    }

    // 被克制的情况
    if ((attackerElement == Element::FIRE && targetElement == Element::WATER) ||
        (attackerElement == Element::WATER && targetElement == Element::EARTH) ||
        (attackerElement == Element::EARTH && targetElement == Element::AIR) ||
        (attackerElement == Element::AIR && targetElement == Element::FIRE)) {
        return 0.8f;  // 被克制，伤害减少
    }

    return 1.0f;  // 默认返回 1，表示没有元素相克
}

Player::Player(float health, Element element)
    : Entities(health, element), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr) {}


Player::Player() : Entities(100, Element::WATER), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr) {}


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

void Player::attack(Entities& target) {
    // 默认伤害
    float damage = 10.0f;
    if (weapon != nullptr) {
        damage = weapon->getPower();
    }

    // 考虑属性相克
    float elementModifier = calculateElementalDamageModifier(element, target.getElement());

    // 计算最终伤害
    damage *= elementModifier;

    // 给目标造成伤害
    target.takeDamage(damage);
}

void Player::castSkill() {
    CCLOG("Player casts a special skill!");
}

void Player::printStatus() {
    Entities::printStatus();
    CCLOG("Experience: %d", experience);
    CCLOG("Level: %d", level);

    // 打印装备信息
    if (weapon != nullptr) {
        CCLOG("Equipped Weapon: %s", weapon->getName().c_str());
    }
    else {
        CCLOG("No Weapon Equipped");
    }

    if (armor != nullptr) {
        CCLOG("Equipped Armor: %s", armor->getName().c_str());
    }
    else {
        CCLOG("No Armor Equipped");
    }

    if (accessory != nullptr) {
        CCLOG("Equipped Accessory: %s", accessory->getName().c_str());
    }
    else {
        CCLOG("No Accessory Equipped");
    }
}

void Player::equipWeapon(Weapon* newWeapon) {
    if (newWeapon != nullptr) {
        weapon = newWeapon;
        CCLOG("Equipped Weapon: %s", weapon->getName().c_str());
    }
}

void Player::equipArmor(Armor* newArmor) {
    if (newArmor != nullptr) {
        armor = newArmor;
        CCLOG("Equipped Armor: %s", armor->getName().c_str());
    }
}

void Player::equipAccessory(Accessory* newAccessory) {
    if (newAccessory != nullptr) {
        accessory = newAccessory;
        CCLOG("Equipped Accessory: %s", accessory->getName().c_str());
    }
}

Weapon* Player::getWeapon() const {
    return weapon;
}

Armor* Player::getArmor() const {
    return armor;
}

Accessory* Player::getAccessory() const {
    return accessory;
}
