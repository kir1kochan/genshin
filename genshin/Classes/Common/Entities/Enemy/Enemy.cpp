#include "Enemy.h"
#include "../Player/Player.h"
#include <vector>

// ���캯��
Enemy::Enemy(float health, float attack, float defence, Element element, float attackRange, int aggressionLevel, float detectionRadius,
     int baseLevel, const std::string& imagePath, int drop)
    : Entities(health, attack, defence, element, attackRange),  
    aggressionLevel(aggressionLevel),
    detectionRadius(detectionRadius),
    baseLevel(baseLevel),
    imagePath(imagePath),
    spriteGenerated(false),
    isAlive(true), // Ĭ������Ϊ���
    drop(drop){} 

// Ĭ�Ϲ��캯��
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE, 30.0f),// Ĭ�Ϲ�����ΧΪ2
    aggressionLevel(1),
    detectionRadius(100.0f),
    baseLevel(1),
    imagePath(""),
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
    return imagePath;
}

// �ܵ��˺�
void Enemy::takeDamage(float amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
        setIsAlive(false);
        sprite->stopActionByTag(100);

        // ������˸������2������˸5��
        auto blinkAction = Blink::create(1.0f, 5);

        // ��˸��ɺ�Ļص����ù�����ʧ
        auto disappearCallback = CallFunc::create([this]() {
            // ������Ӹ��ڵ��Ƴ�
            sprite->removeFromParent();
            this->removeFromParent(); // �Ƴ����˽ڵ㱾��
            CCLOG("Enemy has died.");
            });

        // ��϶�������˸ -> ��ʧ
        auto deathSequence = Sequence::create(blinkAction, disappearCallback, nullptr);

        // ���ж���
        sprite->runAction(deathSequence);

    }

    drawProgressBar(barNode, health / maxHealth);
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
        if (baseLevel) {
            player->gainExperience(30 + 2 * baseLevel);
            player->getBackpack()->addCoins(30 + 2 * baseLevel);
            baseLevel = 0;
        }
        return;
    }
    
    // �������������������
    if (distance > detectionRadius) {
        return;
    }
    //  �����ʱ������Ի��߱���״̬�����޷��ƶ��͹���
    if (this->hasStatusEffect("Frozen") || this->hasStatusEffect("Paralyzed")) {
        return;
    }
    bool isAttacking = false;
    // ������˺����֮�������
    cocos2d::Vec2 enemyPosition = getPosition();
    cocos2d::Vec2 playerPosition = player->getPosition();
    cocos2d::Vec2 directionToPlayer = playerPosition - enemyPosition;

    // ������˳��򣨻��ڷ���
    cocos2d::Vec2 normalizedDirection = directionToPlayer.getNormalized();
    float angle = normalizedDirection.getAngle();  // ��ȡ�Ƕȣ����ȣ�

    // ����������С�ڹ�����Χ�����˷�������
    if (distance < attackRange) {
        if (canAttack()) {
            attackTarget(*player);  // �����й�������
            CCLOG("Enemy attacks player, distance: %.2f", distance);
            isAttacking = true;
            // ֹͣ��ǰ����
            sprite->stopActionByTag(100);

            // ִ�й���������ֻ����һ�Σ�
            auto attackAnimate = getDirectionAnimate(angle, isAttacking);
            auto resetToNormal = CallFunc::create([this, angle]() {
                auto repeatNormalAnimate = RepeatForever::create(getDirectionAnimate(angle, false));
                repeatNormalAnimate->setTag(100); // ����������Tag
                sprite->runAction(repeatNormalAnimate); // ���¿�ʼ�ƶ�����
                });

            // ʹ�� Sequence ���������������� + ���ö���
            auto attackSequence = Sequence::create(attackAnimate, resetToNormal, nullptr);
            sprite->runAction(attackSequence);
            return;
        }
    }
    // �����ҵȼ����ڵ��ˣ����˿��ܻ������ӱ���Ϊ
    else if (player->getLevel() > baseLevel * 2) {
        CCLOG("Enemy is fleeing, player's level is too high.");
        // �����߼�
        // �������ܷ������ܷ���������λ�õķ���
        cocos2d::Vec2 fleeDirection = -directionToPlayer.getNormalized();  // ���򣬵�λ��
        angle *= -1;
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
    auto repeatAnimate = RepeatForever::create(getDirectionAnimate(angle,isAttacking));
    repeatAnimate->setTag(100); // �������ֹͣ
    sprite->stopActionByTag(100);
    sprite->runAction(repeatAnimate);
}

// ��ӡ����״̬
void Enemy::printStatus() {
    Entities::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Attack Range: %.2f", attackRange); // ��ʾ������Χ
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Sprite Filename: %s", imagePath.c_str());
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
    doc.AddMember("imagePath", rapidjson::Value(imagePath.c_str(), allocator), allocator);
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
    if (doc.HasMember("imagePath")) imagePath = doc["imagePath"].GetString();
    if (doc.HasMember("isAlive")) isAlive = doc["isAlive"].GetBool(); // ���ش��״̬
}

// ������λ�õ�clone
Enemy* Enemy::clone(const cocos2d::Vec2& newPosition) {
    Enemy* newEnemy = new Enemy(health, attack, defence, element, aggressionLevel, detectionRadius, attackRange, baseLevel, imagePath, drop);
    newEnemy->setPosition(newPosition);
    return newEnemy;
}

// �����ļ������ɾ���
cocos2d::Sprite* Enemy::generateSprite() {
    if (!spriteGenerated) {
        // ��������
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(imagePath);

        // ÿ֡�ĳߴ�
        int frameWidth = 64;
        int frameHeight = 64;


        // ��������Ͷ������� (4������2��״̬������������)
        const int rows = 4;  // 4������
        const int cols = 6;  // ÿ������6֡ (ǰ��֡Ϊ��ͨ����������֡Ϊ��������)


        // �ü��ĸ�����Ķ���֡
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < 3; ++col) { // ǰ��֡����ͨ����
                Rect rect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
                auto frame = SpriteFrame::createWithTexture(texture, rect);
                frames.pushBack(frame);
            }
            for (int col = 3; col < 6; ++col) { // ����֡�ǹ�������
                Rect rect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
                auto frame = SpriteFrame::createWithTexture(texture, rect);
                attackFrames.pushBack(frame); // �洢����֡
            }
        }

        // �������鲢����Ĭ��֡
        sprite = Sprite::createWithSpriteFrame(frames.front());
        this->addChild(sprite);

        for (int i = 0; i < 4; ++i) {
            Vector<SpriteFrame*> directionFrames;
            for (int j = i * 3; j < (i + 1) * 3; ++j) {
                directionFrames.pushBack(frames.at(j)); // ��ͨ����֡
            }
            auto animation = Animation::createWithSpriteFrames(directionFrames, 0.2f); // 0.2��ÿ֡
            auto animate = Animate::create(animation); // ���� Animate    
            animates.pushBack(animate);
        }

        for (int i = 0; i < 4; ++i) {
            Vector<SpriteFrame*> directionAttackFrames;
            for (int j = i * 3; j < (i + 1) * 3; ++j) {
                directionAttackFrames.pushBack(attackFrames.at(j)); // ��������֡
            }
            auto attackAnimation = Animation::createWithSpriteFrames(directionAttackFrames, 0.2f); // 0.2��ÿ֡
            auto animate = Animate::create(attackAnimation); // ���� Animate
            animates.pushBack(animate);
        }

        auto defaultAnimate = RepeatForever::create(animates.at(2));
        defaultAnimate->setTag(100); // ����������Tag
        // ���ݵ��˳����л�����
        sprite->runAction(defaultAnimate); // Ĭ�ϳ���
        applyElementColor();
        spriteGenerated = true;
        barNode = DrawNode::create();
        this->addChild(barNode);
        barNode->setPosition(-20, 20);
        drawProgressBar(barNode, health / maxHealth);
        return sprite;
    }
    return nullptr; // ��������Ѿ����ɣ����ؿ�ָ��
}

void Enemy::applyElementColor() {
    // ���ݵ��˵�������������ɫ
     // ���ݵ��˵�������������ɫ
    switch (element) {
    case Element::FIRE:
        this->sprite->setColor(cocos2d::Color3B::RED);
        break;
    case Element::WATER:
        this->sprite->setColor(cocos2d::Color3B::BLUE);
        break;
    case Element::EARTH:
        this->sprite->setColor(cocos2d::Color3B(139, 69, 19)); // ����ɫ
        break;
    case Element::ICE:
        this->sprite->setColor(cocos2d::Color3B::WHITE);
        break;
    case Element::THUNDER:
        this->sprite->setColor(cocos2d::Color3B::YELLOW);
        break;
    case Element::AIR:
        this->sprite->setColor(cocos2d::Color3B(144, 238, 144)); // ǳ��ɫ
        break;
    case Element::GRASS:
        this->sprite->setColor(cocos2d::Color3B(34, 139, 34)); // ����ɫ
        break;
    default:
        this->sprite->setColor(cocos2d::Color3B::WHITE);
        break;
    }
}

cocos2d::Animate* Enemy::getDirectionAnimate(float angle, bool isAttacking) {
    if (angle >= -M_PI_4 && angle < M_PI_4) {
        return animates.at(isAttacking ? 7 : 3); // �������� or ��ͨ����
    }
    else if (angle >= M_PI_4 && angle < 3 * M_PI_4) {
        return animates.at(isAttacking ? 4 : 0); // �������� or ��ͨ����
    }
    else if (angle >= -3 * M_PI_4 && angle < -M_PI_4) {
        return animates.at(isAttacking ? 6 : 2); // �������� or ��ͨ����
    }
    else {
        return animates.at(isAttacking ? 5 : 1); // �������� or ��ͨ����
    }
}

void Enemy::setSpriteFilename(std::string filename) {
    imagePath = filename;
}

void Enemy::drawProgressBar(cocos2d::DrawNode* barNode, float percent) {
    barNode->clear(); // ��վɵ�����

    // ȷ���ٷֱ��� 0 �� 1 ֮��
    percent = std::max(0.0f, std::min(1.0f, percent));
    float filledWidth = 40 * percent;

    // ���ƾ��ν�����
    barNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(filledWidth, 5), Color4F::RED);

    // ���Ʊ߿�
    cocos2d::Vec2 origin(0, 0); // ���
    cocos2d::Vec2 topRight(40, 5); // ���Ͻǵ�
    barNode->drawRect(origin, topRight, cocos2d::Color4F::BLACK); // ��ɫ�߿�
}
