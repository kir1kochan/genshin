#include "Player.h"
#include "cocos2d.h"

#include "Classes/Common/Item/Equipment/Weapon/Weapon.h"
#include "Classes/Common/Item/Equipment/Armor/Armor.h"
#include "Classes/Common/Item/Equipment/Accessory/Accessory.h"
#include "Skill/AttackSkill/AttackSkill.h"
#include "Skill/HealSkill/HealSkill.h"
#include "Skill/ShieldSkill/ShieldSkill.h"
#include "Skill/Skill.h"
#include "Classes/Common/Backpack/Backpack.h"
#include <memory>

Player::Player(float health, Element element, float attackRange)
    : Entities(), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), currentShield(0), backpack(){
    skillBar.resize(3, nullptr); // 初始化技能栏为空
}


Player::Player() : Entities(), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), currentShield(0), backpack() {
    skillBar.resize(3, nullptr); // 初始化技能栏为空
}

Player::Player(cocos2d::Sprite* sprite) { 
    Player();
    this->addChild(sprite, 1);
}

void Player::levelUp() {
    level++;
    maxHealth += 20.0f;  // 每次升级增加最大血量
    health = maxHealth;   // 血量恢复
    CCLOG("Level up! Now level %d", level);

    // 解锁新技能暂未实现
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

void Player::chanegElement(Element newElement)
{
    element = newElement;
}

void Player::attackTarget(Enemy& target) {
    
    // 检查攻击是否可以执行（冷却时间）
    if (!canAttack()) {
        CCLOG("Cannot attack yet, cooldown active.");
        return;
    }

    // 检查目标是否在攻击范围内
    if (!attackInRange(target)) {
        CCLOG("Target is out of range.");
        return;
    }
    
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
    // 更新攻击冷却
    updateAttackCooldown(skillCooldownInterval);
}

void Player::attackTargetBySkill(Enemy& target, float attackValue, Element skillElment)
{
    // 检查攻击是否可以执行（冷却时间）
    if (!canAttack()) {
        CCLOG("Cannot attack yet, cooldown active.");
        return;
    }

    // 检查目标是否在攻击范围内
    if (!attackInRange(target)) {
        CCLOG("Target is out of range.");
        return;
    }

    float damage = attackValue;
    

    // 考虑属性相克
    float elementModifier = calculateElementalDamageModifier(skillElment, target.getElement());

    // 计算最终伤害
    damage *= elementModifier;

    // 给目标造成伤害
    target.takeDamage(damage);
    // 更新攻击冷却
    updateAttackCooldown(skillCooldownInterval);
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

void Player::equipWeapon(std::shared_ptr<Weapon> newWeapon) {
    if (newWeapon != nullptr) {
        weapon = newWeapon;
        CCLOG("Equipped Weapon: %s", weapon->getName().c_str());
        attackRange = weapon->getAttackRange();
        CDtoSet = 1.0f / weapon->getAttackSpeed();  // 攻击频率影响攻击冷却时间
    }
}

void Player::equipArmor(std::shared_ptr<Armor> newArmor) {
    if (newArmor != nullptr) {
        armor = newArmor;
        CCLOG("Equipped Armor: %s", armor->getName().c_str());
    }
}

void Player::equipAccessory(std::shared_ptr<Accessory> newAccessory) {
    if (newAccessory != nullptr) {
        accessory = newAccessory;
        CCLOG("Equipped Accessory: %s", accessory->getName().c_str());
    }
}

// 获取武器攻击范围
float Player::getWeaponAttackRange() const {
    return (weapon != nullptr) ? weapon->getAttackRange() : 0.0f;
}

// 获取武器攻击频率
float Player::getWeaponAttackSpeed() const {
    return (weapon != nullptr) ? weapon->getAttackSpeed() : 0.0f;
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

    skillBar[skillSlot] = skill;  // 使用智能指针
    CCLOG("Equipped skill: %s in slot %d", skill->getName().c_str(), skillSlot);
    return true;
}

void Player::unequipSkill(int skillSlot) {
    if (skillSlot < 0 || skillSlot >= 3 || !skillBar[skillSlot]) {
        CCLOG("Invalid or empty skill slot: %d", skillSlot);
        return;
    }

    CCLOG("Unequipped skill: %s from slot %d", skillBar[skillSlot]->getName().c_str(), skillSlot);
    skillBar[skillSlot] = nullptr;  // 智能指针会自动管理内存
}

void Player::useSkill(int skillSlot, Enemy& target) {
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

void Player::addItemToBackpack(int id,int count) {
    backpack.addItem(id, count);
}

void Player::removeItemFromBackpack(int itemId) {
    backpack.removeItem(itemId);  
}

void Player::printBackpackInfo() const {
    backpack.printInfo();
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

void Player::setShield(float shield,float Time) {
    currentShield = shield;
    this->shieldTime = Time;
    CCLOG("Player receives a shield of %.2f.", shield);
}

float Player::getShield() const {
    return currentShield;
}


// 用于定时更新玩家状态
void Player::update(float deltaTime) {
    // 累积技能冷却更新的时间
    skillCooldownAccumulator += deltaTime;
    if (skillCooldownAccumulator >= skillCooldownInterval) {
        updateSkillsCooldown(skillCooldownAccumulator); // 更新技能冷却
        skillCooldownAccumulator = 0.0f;  // 重置时间
    }

    // 累积护盾时间更新的时间
    shieldTimeAccumulator += deltaTime;
    if (shieldTimeAccumulator >= shieldTimeInterval) {
        updateshieldTime(shieldTimeAccumulator);  // 更新护盾时间
        shieldTimeAccumulator = 0.0f;  // 重置时间
    }

    // 累积攻击冷却的时间
    attackCooldownAccumulator += deltaTime;
    if (attackCooldownAccumulator >= attackCooldownInterval) {
        updateAttackCooldown(attackCooldownAccumulator);  // 更新攻击冷却
        attackCooldownAccumulator = 0.0f;  // 重置时间
    }

    // 更新玩家的状态效果
    updateStatusEffects(deltaTime);  // 更新所有的状态效果（例如中毒、冰冻等）
}

// 将所有成员变量序列化为 JSON 格式，生成 JSON 字符串
std::string Player::saveToJson() const {
    // 调用父类方法，获取基础数据的 JSON 字符串
    rapidjson::Document doc;
    doc.Parse(Entities::saveToJson().c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing base class JSON");
        return "{}";
    }

    auto& allocator = doc.GetAllocator();

    // 序列化 Player 特有数据
    doc.AddMember("experience", experience, allocator);
    doc.AddMember("level", level, allocator);

    // 序列化装备（仅保存指针是否为空）
    doc.AddMember("weapon", weapon ? weapon->getId() : 0, allocator);
    doc.AddMember("armor", armor ? armor->getId() : 0, allocator);
    doc.AddMember("accessory", accessory ? accessory->getId() : 0, allocator);

    // 序列化技能系统
    rapidjson::Value unlockedSkillsArray(rapidjson::kArrayType);
    for (const auto& skill : unlockedSkills) {
        if (skill) {
            unlockedSkillsArray.PushBack(skill->getId(), allocator);
        }
    }
    doc.AddMember("unlockedSkills", unlockedSkillsArray, allocator);

    rapidjson::Value skillBarArray(rapidjson::kArrayType);
    for (const auto& skill : skillBar) {
        if (skill) {
            skillBarArray.PushBack(skill->getId(), allocator);
        }
        else {
            skillBarArray.PushBack(0, allocator); // 空槽位
        }
    }
    doc.AddMember("skillBar", skillBarArray, allocator);

    doc.AddMember("currentShield", currentShield, allocator);

    // 转换为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串中读取成员变量序列
void Player::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // 调用父类方法加载基础数据
    if (doc.IsObject()) {
        Entities::loadFromJson(jsonString);
    }

    // 反序列化 Player 特有数据
    if (doc.HasMember("experience")) experience = doc["experience"].GetInt();
    if (doc.HasMember("level")) level = doc["level"].GetInt();

    // 反序列化装备（通过 ID 恢复指针）
    if (doc.HasMember("weapon")) {
        int weaponid = doc["weapon"].GetInt();
        auto item = backpack.idToItemMap[weaponid];
        weapon = std::dynamic_pointer_cast<Weapon>(item);
        if (!weapon) {
            CCLOG("Failed to cast item to Weapon.");
        }
    }

    if (doc.HasMember("armor")) {
        int armorid = doc["armor"].GetInt();
        auto item = backpack.idToItemMap[armorid];
        armor = std::dynamic_pointer_cast<Armor>(item);
        if (!armor) {
            CCLOG("Failed to cast item to Armor.");
        }
    }

    if (doc.HasMember("accessory")) {
        int accessoryid = doc["accessory"].GetInt();
        auto item = backpack.idToItemMap[accessoryid];
        accessory = std::dynamic_pointer_cast<Accessory>(item);
        if (!accessory) {
            CCLOG("Failed to cast item to Accessory.");
        }
    }

    // 反序列化技能系统
    if (doc.HasMember("unlockedSkills") && doc["unlockedSkills"].IsArray()) {
        unlockedSkills.clear();
        for (const auto& skillId : doc["unlockedSkills"].GetArray()) {
            int skillIdInt = skillId.GetInt();
            std::shared_ptr<Skill> skill = creatSkillById(skillIdInt, jsonString);  // 创建技能并加载
            unlockedSkills.push_back(skill);
        }
    }

    if (doc.HasMember("skillBar") && doc["skillBar"].IsArray()) {
        skillBar.clear();
        for (const auto& skillId : doc["skillBar"].GetArray()) {
            int skillIdInt = skillId.GetInt();
            std::shared_ptr<Skill> skill = creatSkillById(skillIdInt, jsonString);  // 创建技能并加载
            skillBar.push_back(skill);
        }
    }

    if (doc.HasMember("currentShield")) currentShield = doc["currentShield"].GetFloat();
}
std::shared_ptr<Skill> Player::creatSkillById(int id, const std::string& jsonString) {
    int skillType = id / 10000;
    int subType = (id / 100) % 100;

    std::shared_ptr<Skill> skill = nullptr;
    skill = std::make_shared<ShieldSkill>(id, "Shield Skill", 15.0f, 100.0f, 20.0f);

    switch (skillType) {
    case 9:
        switch (subType) {
        case 1:
            skill = std::make_shared<AttackSkill>(id, "Attack Skill", 10.0f, 50.0f, 10.0f, Element::FIRE);
            break;
        case 2:
            skill = std::make_shared<HealSkill>(id, "Heal Skill", 5.0f, 30.0f);
            break;
        case 3:
            skill = std::make_shared<ShieldSkill>(id, "Shield Skill", 15.0f, 100.0f, 20.0f);
            break;
        default:
            CCLOG("Unknown skill subtype %d", subType);
            break;
        }
        break;
    default:
        CCLOG("Unknown skill type %d", skillType);
        break;
    }
    
    if (skill) {
        skill->loadFromJson(jsonString);  // 加载技能的其他数据
    }

    return skill;
}