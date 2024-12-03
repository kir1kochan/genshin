#include "Enemy.h"

// ���캯��
Enemy::Enemy(float health, float attack, float defence, Element element, float attackRange, int aggressionLevel, float detectionRadius,
     int baseLevel, const std::string& spriteFilename, int drop)
    : Entities(health, attack, defence, element, attackRange),  
    aggressionLevel(aggressionLevel),
    detectionRadius(detectionRadius),
    baseLevel(baseLevel),
    spriteFilename(spriteFilename),
    spriteGenerated(false),
    isAlive(true), // Ĭ������Ϊ���
    drop(drop){} 

// Ĭ�Ϲ��캯��
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE,2.0f),// Ĭ�Ϲ�����ΧΪ2
    aggressionLevel(1),
    detectionRadius(10.0f),
    baseLevel(1),
    spriteFilename(""),
    spriteGenerated(false),
    isAlive(true), // Ĭ��Ϊ���
    drop(0){} // Ĭ������Ϊ�޵���

// ��ȡ�����Եȼ�
int Enemy::getAggressionLevel() const {
    return aggressionLevel;
}

// ��ȡ���а뾶
float Enemy::getDetectionRadius() const {
    return detectionRadius;
}

// ��ȡ��׼�ȼ�
int Enemy::getBaseLevel() const {
    return baseLevel;
}

// ��ȡ�����ļ���
std::string Enemy::getSpriteFilename() const {
    return spriteFilename;
}

// �ܵ��˺�
void Enemy::takeDamage(float amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
        setIsAlive(false);
    }
    currentCooldown = 0.5; // �ܻ��Ĺ�����ֱ
}

// ��ȡ���״̬
bool Enemy::getIsAlive() const {
    return isAlive;
}

// ���ô��״̬
void Enemy::setIsAlive(bool alive) {
    isAlive = alive;
}

// ��������
void Enemy::attackTarget(Entities& target, float amount=0, Element element1= Element::FIRE) {
    // ʹ�û���Ĺ�����Χ���
    if (attackInRange(target)) {
        float elementModifier = calculateElementalDamageModifier(element, target.getElement());
        target.takeDamage(elementModifier * attack * aggressionLevel);
        currentCooldown = 5.0f / aggressionLevel;
    }
    else {
        CCLOG("Target is out of range.");
    }
}

// ����AI��Ϊ
void Enemy::aiBehavior(float distance, Player* player) {
    if (!isAlive) {
        // ���е���������䲢�������������Ϊ0
        if (drop) {
            player->addItemToBackpack(drop, 1);
            drop = 0;
        }
        return;
    }
    
    // �������������������
    if (distance > detectionRadius) {
        return;
    }

    // ������˺����֮�������
    cocos2d::Vec2 enemyPosition = getPosition();
    cocos2d::Vec2 playerPosition = player->getPosition();
    cocos2d::Vec2 directionToPlayer = playerPosition - enemyPosition;

    // ����������С�ڹ�����Χ�����˷�������
    if (distance < attackRange) {
        if (canAttack()) {
            attackTarget(*player);  // �����й�������
            CCLOG("Enemy attacks player, distance: %.2f", distance);
        }
    }
    // �����ҵȼ����ڵ��ˣ����˿��ܻ������ӱ���Ϊ
    else if (player->getLevel() > baseLevel * 2) {
        CCLOG("Enemy is fleeing, player's level is too high.");
        // �����߼�
        // �������ܷ������ܷ���������λ�õķ���
        cocos2d::Vec2 fleeDirection = -directionToPlayer.getNormalized();  // ���򣬵�λ��

        // �ƶ�����
        float fleeSpeed = 200.0f; // �����ٶ�
        setPosition(enemyPosition + fleeDirection * fleeSpeed * 0.1f);  // ÿ֡���ٶ��ƶ�
    }
    else {
        CCLOG("Enemy is pursuing player, distance: %.2f", distance);
        // ׷���߼������˼���������ƶ�
        // ����׷������
        cocos2d::Vec2 pursuitDirection = directionToPlayer.getNormalized();  // ��λ��
         // �ƶ�����
        float pursuitSpeed = 100.0f * aggressionLevel; // ׷���ٶ�
        setPosition(enemyPosition + pursuitDirection * pursuitSpeed * 0.1f);  // ÿ֡���ٶ��ƶ�
    }
}

// ��ӡ����״̬
void Enemy::printStatus() {
    Entities::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Attack Range: %.2f", attackRange); // ��ʾ������Χ
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Sprite Filename: %s", spriteFilename.c_str());
    CCLOG("Is Alive: %s", isAlive ? "Yes" : "No");  // ��ӡ���״̬
}

// �����˶������л�Ϊ JSON �ַ���
std::string Enemy::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    std::string baseJson = Entities::saveToJson();
    rapidjson::Document baseDoc;
    baseDoc.Parse(baseJson.c_str());

    doc.AddMember("health", baseDoc["health"], allocator);
    doc.AddMember("maxHealth", baseDoc["maxHealth"], allocator);
    doc.AddMember("attack", baseDoc["attack"], allocator);
    doc.AddMember("defence", baseDoc["defence"], allocator);
    doc.AddMember("element", baseDoc["element"], allocator);

    doc.AddMember("aggressionLevel", aggressionLevel, allocator);
    doc.AddMember("detectionRadius", detectionRadius, allocator);
    doc.AddMember("attackRange", attackRange, allocator); // ��ӹ�����Χ
    doc.AddMember("baseLevel", baseLevel, allocator);
    doc.AddMember("spriteFilename", rapidjson::Value(spriteFilename.c_str(), allocator), allocator);
    doc.AddMember("isAlive", isAlive, allocator); // ������״̬

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// �� JSON �ַ�����������
void Enemy::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    Entities::loadFromJson(jsonString);

    if (doc.HasMember("aggressionLevel")) aggressionLevel = doc["aggressionLevel"].GetInt();
    if (doc.HasMember("detectionRadius")) detectionRadius = doc["detectionRadius"].GetFloat();
    if (doc.HasMember("attackRange")) attackRange = doc["attackRange"].GetFloat();  // ���ع�����Χ
    if (doc.HasMember("baseLevel")) baseLevel = doc["baseLevel"].GetInt();
    if (doc.HasMember("spriteFilename")) spriteFilename = doc["spriteFilename"].GetString();
    if (doc.HasMember("isAlive")) isAlive = doc["isAlive"].GetBool(); // ���ش��״̬
}

// ������λ�õ�clone
Enemy* Enemy::clone(const cocos2d::Vec2& newPosition) {
    Enemy* newEnemy = new Enemy(health, attack, defence, element, aggressionLevel, detectionRadius, attackRange, baseLevel, spriteFilename, drop);
    newEnemy->setPosition(newPosition);
    return newEnemy;
}

// �����ļ������ɾ���
cocos2d::Sprite* Enemy::generateSprite() {
    if (!spriteGenerated) {
        auto sprite = cocos2d::Sprite::create(spriteFilename);
        if (sprite) {
            spriteGenerated = true;  // �������ɳɹ�����Ϊ true
            return sprite;
        }
    }
    return nullptr;  // ��������Ѿ����ɻ�ʧ�ܣ����ؿ�ָ��
}
