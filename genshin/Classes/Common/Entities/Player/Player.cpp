#include "Player.h"
#include "cocos2d.h"

Player::Player(float health, Element element)
    : Entities(health, element), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr) {}


Player::Player() : Entities(100, Element::WATER), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr) {}


void Player::levelUp() {
    level++;
    maxHealth += 20.0f;  // ÿ�������������Ѫ��
    health = maxHealth;   // Ѫ���ָ�
    CCLOG("Level up! Now level %d", level);
}

int Player::getLevel() const {
    return level;
}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {  // ����100����ֵ����
        experience = 0;
        levelUp();
    }
}

void Player::attack(Entities& target) {
    // Ĭ���˺�
    float damage = 10.0f;
    if (weapon != nullptr) {
        damage = weapon->getPower();
    }

    // �����������
    float elementModifier = calculateElementalDamageModifier(element, target.getElement());

    // ���������˺�
    damage *= elementModifier;

    // ��Ŀ������˺�
    target.takeDamage(damage);
}

void Player::castSkill() {
    CCLOG("Player casts a special skill!");
}

void Player::printStatus() {
    Entities::printStatus();
    CCLOG("Experience: %d", experience);
    CCLOG("Level: %d", level);

    // ��ӡװ����Ϣ
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
