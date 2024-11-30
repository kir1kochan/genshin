#include "Player.h"
#include "cocos2d.h"

Player::Player(float health, Element element)
    : Entities(health, element), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), activeShield(0) {
    skillBar.resize(3, nullptr); // 初始化技能栏为空
}


Player::Player() : Entities(100, Element::WATER), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), activeShield(0) {
    skillBar.resize(3, nullptr); // 初始化技能栏为空
}

void Player::levelUp() {
    level++;
    maxHealth += 20.0f;  // 每次升级增加最大血量
    health = maxHealth;   // 血量恢复
    CCLOG("Level up! Now level %d", level);

    // 解锁新技能
    if (level == 2) unlockSkill(std::make_shared<FireballSkill>());
    if (level == 3) unlockSkill(std::make_shared<ShieldSkill>());
    if (level == 4) unlockSkill(std::make_shared<HealSkill>());
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

void Player::unlockSkill(const std::shared_ptr<Skill>& newSkill) {
    unlockedSkills.push_back(newSkill);
    CCLOG("Unlocked skill: %s", newSkill->getName().c_str());
}

bool Player::equipSkill(int skillSlot, const std::shared_ptr<Skill>& skill) {
    if (skillSlot < 0 || skillSlot >= 3) {
        CCLOG("Invalid skill slot: %d", skillSlot);
        return false;
    }

    if (std::find(skillBar.begin(), skillBar.end(), skill) != skillBar.end()) {
        CCLOG("Skill %s is already equipped.", skill->getName().c_str());
        return false; // 不能重复装备
    }

    skillBar[skillSlot] = skill;
    CCLOG("Equipped skill: %s in slot %d", skill->getName().c_str(), skillSlot);
    return true;
}

void Player::unequipSkill(int skillSlot) {
    if (skillSlot < 0 || skillSlot >= 3 || !skillBar[skillSlot]) {
        CCLOG("Invalid or empty skill slot: %d", skillSlot);
        return;
    }

    CCLOG("Unequipped skill: %s from slot %d", skillBar[skillSlot]->getName().c_str(), skillSlot);
    skillBar[skillSlot] = nullptr;
}

void Player::useSkill(int skillSlot, Entities& target) {
    if (skillSlot < 0 || skillSlot >= 3 || !skillBar[skillSlot]) {
        CCLOG("Invalid or empty skill slot: %d", skillSlot);
        return;
    }

    auto skill = skillBar[skillSlot];
    if (skill->isOnCooldown()) {
        CCLOG("Skill %s is on cooldown.", skill->getName().c_str());
        return;
    }

    skill->activate(this, target);
    skill->resetCooldown();
}

void Player::updateSkillsCooldown(float deltaTime) {
    for (const auto& skill : skillBar) {
        if (skill) {
            skill->updateCooldown(deltaTime);
        }
    }
}

void Player::updateshieldTime(float deltaTime)
{
    if (shieldTime > 0.0f) {
        shieldTime -= deltaTime;
        if (shieldTime <= 0.0f || currentShield <= 0.0f) {
            shieldTime = 0.0f;
            currentShield = 0.0f;
            CCLOG("Shield expired.");
        }
    }
}



void Player::takeDamage(float damage) {
    if (currentShield > 0.0f) {
        // 扣除护甲值
        float absorbed = std::min(damage, currentShield);
        currentShield -= absorbed;
        damage -= absorbed;
        CCLOG("Shield absorbs %.2f damage. Remaining shield: %.2f.", absorbed, currentShield);
    }

    if (damage > 0.0f) {
        // 扣除生命值
        Entities::takeDamage(damage);
        CCLOG("Player takes %.2f damage. Current health: %.2f.", damage, health);
    }
    else {
        CCLOG("Shield fully absorbed the damage.");
    }
}

void Player::heal(float healAmount) {
    health += healAmount;
    if (health > maxHealth) 
        health = maxHealth;
    CCLOG("Player heals %.2f health. Current health: %.2f", healAmount, health);
}

void Player::setShield(float shield) {
    currentShield = shield;
    CCLOG("Player receives a shield of %.2f.", shield);
}

float Player::getShield() const {
    return currentShield;
}