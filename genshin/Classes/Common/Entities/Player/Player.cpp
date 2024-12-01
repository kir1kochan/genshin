#include "Player.h"
#include "cocos2d.h"

Player::Player(float health, Element element)
    : Entities(health, element), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), activeShield(0), backpack(nullptr){
    skillBar.resize(3, nullptr); // ��ʼ��������Ϊ��
}


Player::Player() : Entities(100, Element::WATER), experience(0), level(1), weapon(nullptr), armor(nullptr), accessory(nullptr), activeShield(0), backpack(nullptr) {
    skillBar.resize(3, nullptr); // ��ʼ��������Ϊ��
}

void Player::levelUp() {
    level++;
    maxHealth += 20.0f;  // ÿ�������������Ѫ��
    health = maxHealth;   // Ѫ���ָ�
    CCLOG("Level up! Now level %d", level);

    // �����¼���
    if (level == 2) unlockSkill(std::make_shared<FireballSkill>());
    if (level == 3) unlockSkill(std::make_shared<ShieldSkill>());
    if (level == 4) unlockSkill(std::make_shared<HealSkill>());
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

void Player::unlockSkill(const std::shared_ptr<Skill>& newSkill) {
    unlockedSkills.push_back(newSkill);
    CCLOG("Unlocked skill: %s", newSkill->getName().c_str());
}

bool Player::equipSkill(int skillSlot, const std::shared_ptr<Skill>& skill) {
    if (skillSlot < 0 || skillSlot >= 3) {
        CCLOG("Invalid skill slot: %d", skillSlot);
        return false;
    }

    // ��鼼���Ƿ���װ��
    auto it = std::find(skillBar.begin(), skillBar.end(), skill);
    if (it != skillBar.end()) {
        if (it == skillBar.begin() + skillSlot) {
            CCLOG("Skill %s is already equipped in the same slot.", skill->getName().c_str());
            return false; // ��������Ŀ���λ
        }

        // ������װ�����ڲ�ͬ��λ������
        std::iter_swap(it, skillBar.begin() + skillSlot);
        CCLOG("Swapped skill %s to slot %d.", skill->getName().c_str(), skillSlot);
        return true;
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
void Player::addItemToBackpack(Item* item) {
    backpack.addItem(item); 
}

void Player::removeItemFromBackpack(int itemId) {
    backpack.removeItem(itemId);  
}

void Player::printBackpackInfo() const {
    backpack.printInfo();  

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

void Player::setShield(float shield) {
    currentShield = shield;
    CCLOG("Player receives a shield of %.2f.", shield);
}

float Player::getShield() const {
    return currentShield;
}

// ���ڶ�ʱ�������״̬
void update(float deltaTime) {
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

    // ���汳������
    doc.AddMember("backpack", rapidjson::Value(backpack.saveToJson().c_str(), allocator), allocator);

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
    //�����л�����
    if (doc.HasMember("backpack"))backpack.loadFromJson(doc["backpack"].GetString());  

    // �����л� Player ��������
    if (doc.HasMember("experience")) experience = doc["experience"].GetInt();
    if (doc.HasMember("level")) level = doc["level"].GetInt();

    // �����л�װ����ͨ�� ID �ָ�ָ�룩
    if (doc.HasMember("weapon")) weapon = Weapon::findById(doc["weapon"].GetInt());
    if (doc.HasMember("armor")) armor = Armor::findById(doc["armor"].GetInt());
    if (doc.HasMember("accessory")) accessory = Accessory::findById(doc["accessory"].GetInt());

    // �����л�����ϵͳ
    if (doc.HasMember("unlockedSkills") && doc["unlockedSkills"].IsArray()) {
        unlockedSkills.clear();
        for (const auto& skillId : doc["unlockedSkills"].GetArray()) {
            unlockedSkills.push_back(Skill::findById(skillId.GetInt()));
        }
    }

    if (doc.HasMember("skillBar") && doc["skillBar"].IsArray()) {
        skillBar.clear();
        for (const auto& skillId : doc["skillBar"].GetArray()) {
            skillBar.push_back(skillId.GetInt() == 0 ? nullptr : Skill::findById(skillId.GetInt()));
        }
    }

    if (doc.HasMember("currentShield")) currentShield = doc["currentShield"].GetFloat();
}
