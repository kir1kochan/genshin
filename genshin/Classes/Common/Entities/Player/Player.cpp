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
    skillBar.resize(3, nullptr); // ��ʼ��������Ϊ��
}


Player::Player() : Entities(), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), currentShield(0), backpack() {
    skillBar.resize(3, nullptr); // ��ʼ��������Ϊ��
}

Player::Player(cocos2d::Sprite* sprite) { 
    Player();
    this->addChild(sprite, 1);
}

void Player::levelUp() {
    level++;
    maxHealth += 20.0f;  // ÿ�������������Ѫ��
    health = maxHealth;   // Ѫ���ָ�
    CCLOG("Level up! Now level %d", level);

    // �����¼�����δʵ��
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

void Player::chanegElement(Element newElement)
{
    element = newElement;
}

void Player::attackTarget(Enemy& target) {
    
    // ��鹥���Ƿ����ִ�У���ȴʱ�䣩
    if (!canAttack()) {
        CCLOG("Cannot attack yet, cooldown active.");
        return;
    }

    // ���Ŀ���Ƿ��ڹ�����Χ��
    if (!attackInRange(target)) {
        CCLOG("Target is out of range.");
        return;
    }
    
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
    // ���¹�����ȴ
    updateAttackCooldown(skillCooldownInterval);
}

void Player::attackTargetBySkill(Enemy& target, float attackValue, Element skillElment)
{
    // ��鹥���Ƿ����ִ�У���ȴʱ�䣩
    if (!canAttack()) {
        CCLOG("Cannot attack yet, cooldown active.");
        return;
    }

    // ���Ŀ���Ƿ��ڹ�����Χ��
    if (!attackInRange(target)) {
        CCLOG("Target is out of range.");
        return;
    }

    float damage = attackValue;
    

    // �����������
    float elementModifier = calculateElementalDamageModifier(skillElment, target.getElement());

    // ���������˺�
    damage *= elementModifier;

    // ��Ŀ������˺�
    target.takeDamage(damage);
    // ���¹�����ȴ
    updateAttackCooldown(skillCooldownInterval);
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

void Player::equipWeapon(std::shared_ptr<Weapon> newWeapon) {
    if (newWeapon != nullptr) {
        weapon = newWeapon;
        CCLOG("Equipped Weapon: %s", weapon->getName().c_str());
        attackRange = weapon->getAttackRange();
        CDtoSet = 1.0f / weapon->getAttackSpeed();  // ����Ƶ��Ӱ�칥����ȴʱ��
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

// ��ȡ����������Χ
float Player::getWeaponAttackRange() const {
    return (weapon != nullptr) ? weapon->getAttackRange() : 0.0f;
}

// ��ȡ��������Ƶ��
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

    skillBar[skillSlot] = skill;  // ʹ������ָ��
    CCLOG("Equipped skill: %s in slot %d", skill->getName().c_str(), skillSlot);
    return true;
}

void Player::unequipSkill(int skillSlot) {
    if (skillSlot < 0 || skillSlot >= 3 || !skillBar[skillSlot]) {
        CCLOG("Invalid or empty skill slot: %d", skillSlot);
        return;
    }

    CCLOG("Unequipped skill: %s from slot %d", skillBar[skillSlot]->getName().c_str(), skillSlot);
    skillBar[skillSlot] = nullptr;  // ����ָ����Զ������ڴ�
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
        // �۳�����ֵ
        float absorbed = std::min(damage, currentShield);
        currentShield -= absorbed;
        damage -= absorbed;
        CCLOG("Shield absorbs %.2f damage. Remaining shield: %.2f.", absorbed, currentShield);
    }

    if (damage > 0.0f) {
        // �۳�����ֵ
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


// ���ڶ�ʱ�������״̬
void Player::update(float deltaTime) {
    // �ۻ�������ȴ���µ�ʱ��
    skillCooldownAccumulator += deltaTime;
    if (skillCooldownAccumulator >= skillCooldownInterval) {
        updateSkillsCooldown(skillCooldownAccumulator); // ���¼�����ȴ
        skillCooldownAccumulator = 0.0f;  // ����ʱ��
    }

    // �ۻ�����ʱ����µ�ʱ��
    shieldTimeAccumulator += deltaTime;
    if (shieldTimeAccumulator >= shieldTimeInterval) {
        updateshieldTime(shieldTimeAccumulator);  // ���»���ʱ��
        shieldTimeAccumulator = 0.0f;  // ����ʱ��
    }

    // �ۻ�������ȴ��ʱ��
    attackCooldownAccumulator += deltaTime;
    if (attackCooldownAccumulator >= attackCooldownInterval) {
        updateAttackCooldown(attackCooldownAccumulator);  // ���¹�����ȴ
        attackCooldownAccumulator = 0.0f;  // ����ʱ��
    }

    // ������ҵ�״̬Ч��
    updateStatusEffects(deltaTime);  // �������е�״̬Ч���������ж��������ȣ�
}

// �����г�Ա�������л�Ϊ JSON ��ʽ������ JSON �ַ���
std::string Player::saveToJson() const {
    // ���ø��෽������ȡ�������ݵ� JSON �ַ���
    rapidjson::Document doc;
    doc.Parse(Entities::saveToJson().c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing base class JSON");
        return "{}";
    }

    auto& allocator = doc.GetAllocator();

    // ���л� Player ��������
    doc.AddMember("experience", experience, allocator);
    doc.AddMember("level", level, allocator);

    // ���л�װ����������ָ���Ƿ�Ϊ�գ�
    doc.AddMember("weapon", weapon ? weapon->getId() : 0, allocator);
    doc.AddMember("armor", armor ? armor->getId() : 0, allocator);
    doc.AddMember("accessory", accessory ? accessory->getId() : 0, allocator);

    // ���л�����ϵͳ
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
            skillBarArray.PushBack(0, allocator); // �ղ�λ
        }
    }
    doc.AddMember("skillBar", skillBarArray, allocator);

    doc.AddMember("currentShield", currentShield, allocator);

    // ת��Ϊ JSON �ַ���
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// �� JSON �ַ����ж�ȡ��Ա��������
void Player::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // ���ø��෽�����ػ�������
    if (doc.IsObject()) {
        Entities::loadFromJson(jsonString);
    }

    // �����л� Player ��������
    if (doc.HasMember("experience")) experience = doc["experience"].GetInt();
    if (doc.HasMember("level")) level = doc["level"].GetInt();

    // �����л�װ����ͨ�� ID �ָ�ָ�룩
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

    // �����л�����ϵͳ
    if (doc.HasMember("unlockedSkills") && doc["unlockedSkills"].IsArray()) {
        unlockedSkills.clear();
        for (const auto& skillId : doc["unlockedSkills"].GetArray()) {
            int skillIdInt = skillId.GetInt();
            std::shared_ptr<Skill> skill = creatSkillById(skillIdInt, jsonString);  // �������ܲ�����
            unlockedSkills.push_back(skill);
        }
    }

    if (doc.HasMember("skillBar") && doc["skillBar"].IsArray()) {
        skillBar.clear();
        for (const auto& skillId : doc["skillBar"].GetArray()) {
            int skillIdInt = skillId.GetInt();
            std::shared_ptr<Skill> skill = creatSkillById(skillIdInt, jsonString);  // �������ܲ�����
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
        skill->loadFromJson(jsonString);  // ���ؼ��ܵ���������
    }

    return skill;
}