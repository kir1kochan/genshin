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

Player::Player( Element element, float attackRange)
    :Entities(), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), currentShield(0), maxStamina(100.0f), stamina(100.0f), backpack() {
    skillBar.resize(3, nullptr); // 初始化技能栏为空
}


Player::Player() : Entities(), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), currentShield(0), maxStamina(100.0f), stamina(100.0f), backpack() {
    skillBar.resize(3, nullptr); // 初始化技能栏为空
}

Player::Player(cocos2d::Sprite* sprite)
    : Entities(), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr),
    currentShield(0), maxStamina(100.0f), stamina(100.0f), backpack() {
    skillBar.resize(3, nullptr); // 初始化技能栏为空

    if (sprite != nullptr) {
        this->addChild(sprite, 1);  // 添加到当前节点
    }
}


void Player::levelUp() {
    level++;
    maxHealth += 20.0f + (level / 5);  // 随等级增加血量增幅
    health = maxHealth;                // 升级时血量恢复
    maxStamina += 20.0f + (level / 5);  // 随等级增加体力值增幅
    stamina = maxStamina;           // 升级时体力值恢复
    attack += 5.0f + (level / 3); // 随等级增加攻击力
    CCLOG("Level up! Now level %d", level);

    // 根据当前等级解锁技能
    checkAndUnlockSkills();
}

int Player::getLevel() const {
    return level;
}

void Player::gainExperience(int exp) {
    experience += exp;
    int expToLevelUp = getExperienceForNextLevel();
    if (experience >= expToLevelUp) {
        experience -= expToLevelUp;
        levelUp();
    }
}

float Player::getExperienceForNextLevel() const
{
    return 100 + (level * 10);  // 每次升级，所需经验逐渐增加
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
    float damage = attack;
   

    // 考虑属性相克
    float elementModifier = calculateElementalDamageModifier(element, target.getElement());

    // 计算最终伤害
    damage *= elementModifier;

    // 给目标造成伤害
    target.takeDamage(damage);

    //持对方元素的饰品，可以有吸血效果
    if (accessory != nullptr && target.getElement() == accessory->getElement()) {
        this->heal(damage *= accessory->getPower());
    }

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

    elementModifier= calculateElementalDamageModifier(element, target.getElement());      //技能基础上加上玩家的基础伤害，保证技能的伤害高于普通攻击
  
    damage += (elementModifier * attack);

    // 给目标造成伤害
    target.takeDamage(damage);
    // 更新攻击冷却
    updateAttackCooldown(skillCooldownInterval);
    // 增加组合技能的逻辑和技能元素的特殊效果
    applyElementalEffects(target, skillElment);
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
        attack += weapon->getPower();
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

// 脱下武器
void Player::unequipWeapon() {
    if (weapon != nullptr) {
        CCLOG("Unequipped Weapon: %s", weapon->getName().c_str());
        attack -= weapon->getPower();
        attackRange = 0.0f;  // 恢复原来的攻击范围
        CDtoSet = 0.0f;  // 恢复原来的攻击冷却时间
        weapon = nullptr;  // 解除装备引用
    }
}

// 脱下护甲
void Player::unequipArmor() {
    if (armor != nullptr) {
        CCLOG("Unequipped Armor: %s", armor->getName().c_str());
        armor = nullptr;  // 解除装备引用
    }
}

// 脱下饰品
void Player::unequipAccessory() {
    if (accessory != nullptr) {
        CCLOG("Unequipped Accessory: %s", accessory->getName().c_str());
        accessory = nullptr;  // 解除装备引用
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
    if (skillSlot < 0 || skillSlot >= 4) {
        CCLOG("Invalid skill slot: %d", skillSlot);
        return false;
    }

    skillBar[skillSlot] = skill;  // 使用智能指针
    CCLOG("Equipped skill: %s in slot %d", skill->getName().c_str(), skillSlot);
    return true;
}

void Player::unequipSkill(int skillSlot) {
    if (skillSlot < 0 || skillSlot >= 4 || !skillBar[skillSlot]) {
        CCLOG("Invalid or empty skill slot: %d", skillSlot);
        return;
    }

    CCLOG("Unequipped skill: %s from slot %d", skillBar[skillSlot]->getName().c_str(), skillSlot);
    skillBar[skillSlot] = nullptr;  // 智能指针会自动管理内存
}

void Player::useSkill(int skillSlot, Enemy& target) {
    if (skillSlot < 0 || skillSlot >= 4 || !skillBar[skillSlot]) {
        CCLOG("Invalid or empty skill slot: %d", skillSlot);
        return;
    }

    auto skill = skillBar[skillSlot];  
    if (skill->isOnCooldown()) {
        CCLOG("Skill %s is on cooldown.", skill->getName().c_str());
        return;
    }
    // 检查体力是否足够
    float requiredStamina = skill->getStaminaCost(); //获取技能消耗的体力

    if (stamina < requiredStamina) {
        CCLOG("Not enough stamina to use skill.");
        return;
    }

    // 执行技能
    reduceStamina(requiredStamina); // 使用技能后减少体力
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

void Player::checkAndUnlockSkills()
{
    // allSkills 存储技能解锁等级与技能
    static std::map<int, std::shared_ptr<Skill>> allSkills = {
     {5, std::make_shared<AttackSkill>(910101, "Fireball", 3.0f, 25.0f, 80.0f, 12.0f, static_cast<Element>(0))},  // 火焰攻击
     {10, std::make_shared<AttackSkill>(910102, "Water Blast", 4.0f, 20.0f, 70.0f, 14.0f, static_cast<Element>(1))},  // 水之冲击
     {15, std::make_shared<AttackSkill>(910103, "Thunderstrike", 5.0f, 30.0f, 85.0f, 18.0f, static_cast<Element>(4))},  // 雷电攻击
     {20, std::make_shared<AttackSkill>(910104, "Earthquake", 6.0f, 35.0f, 90.0f, 10.0f, static_cast<Element>(2))},  // 地震攻击
     {25, std::make_shared<AttackSkill>(910105, "Air Blast", 3.5f, 22.0f, 75.0f, 16.0f, static_cast<Element>(3))},  // 空气冲击
     {30, std::make_shared<AttackSkill>(910106, "Ice Shard", 4.5f, 18.0f, 65.0f, 13.0f, static_cast<Element>(6))},  // 冰霜碎片
     {35, std::make_shared<AttackSkill>(910107, "Vine Lash", 3.0f, 20.0f, 60.0f, 11.0f, static_cast<Element>(5))},  // 藤鞭
     {10, std::make_shared<ShieldSkill>(920101, "Shield Block", 10.0f, 40.0f, 150.0f, 5.0f)},  // 护盾技能
     {6, std::make_shared<HealSkill>(930101, "Healing Touch", 6.0f, 30.0f, 120.0f)}  // 治疗技能
    };

    // 遍历 allSkills，根据玩家等级解锁技能
    for (const auto& skillEntry : allSkills) {
        if (level >= skillEntry.first) {
            // 检查是否已经解锁过技能
            auto it = std::find_if(unlockedSkills.begin(), unlockedSkills.end(),
                [&skillEntry](const std::shared_ptr<Skill>& skill) {
                    return skill->getName() == skillEntry.second->getName();
                });

            if (it == unlockedSkills.end()) { // 如果没有解锁过该技能
                unlockSkill(skillEntry.second);
                for (int i = 0; i < 4; i++) {
                    if (skillBar[i] != nullptr)
                        equipSkill(i, skillEntry.second); //若技能栏没有满，则自动装备技能
                }
            }
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
    if (armor != nullptr) {
        damage *= (500.0f - armor->getPower()) / 500.0f;//加入护甲免伤的逻辑
    }

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
        // 玩家死亡时发送死亡信息
        if (health == 0) {
            cocos2d::EventCustom event("PLAYER_DIED");
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);  // 发送事件
        }
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

void Player::regenerateStamina(float amount) {
    stamina = std::min(stamina + amount, maxStamina); // 体力恢复但不超过最大体力
    CCLOG("Player regenerates %.2f stamina. Current stamina: %.2f", amount, stamina);
}

void Player::reduceStamina(float amount) {
    if (stamina >= amount) {
        stamina -= amount;
        CCLOG("Player loses %.2f stamina. Current stamina: %.2f", amount, stamina);
    }
    else {
        CCLOG("Not enough stamina. Current stamina: %.2f", stamina);
    }
}

float Player::getStamina() const {
    return stamina;
}

void Player::updateStamina(float deltaTime) {
    if (accessory == nullptr)
        // 每帧自动恢复少量体力（例如 0.1 每秒）
        regenerateStamina(0.1f * deltaTime);
    else
        regenerateStamina(accessory->getPower() * deltaTime);
}
// 用于定时更新玩家状态
void Player::update(float deltaTime) {
    // 更新体力
    updateStamina(deltaTime);
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
    doc.AddMember("maxStamina", maxStamina, allocator);
    doc.AddMember("stamina", stamina, allocator);

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
    if (doc.HasMember("maxStamina")) maxStamina = doc["maxStamina"].GetInt();
    if (doc.HasMember("stamina")) stamina = doc["stamina"].GetInt();

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
    int skillType = id / 100000;   // ID的第一部分
    int subType = id % 1000 / 100; // ID的第二部分

    std::shared_ptr<Skill> skill = nullptr;

    switch (skillType) {
    case 9:
        switch (subType) {
        case 1:
            skill = std::make_shared<AttackSkill>(id, "Attack Skill", 10.0f, 5.0f, 50.0f, 10.0f, Element::FIRE);
            break;
        case 2:
            skill = std::make_shared<HealSkill>(id, "Heal Skill", 5.0f,5.0f, 30.0f);
            break;
        case 3:
            skill = std::make_shared<ShieldSkill>(id, "Shield Skill", 15.0f, 5.0f, 100.0f, 20.0f);
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