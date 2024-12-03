#include "Enemy.h"

// 构造函数
Enemy::Enemy(float health, float attack, float defence, Element element, float attackRange, int aggressionLevel, float detectionRadius,
     int baseLevel, const std::string& spriteFilename, int drop)
    : Entities(health, attack, defence, element, attackRange),  
    aggressionLevel(aggressionLevel),
    detectionRadius(detectionRadius),
    baseLevel(baseLevel),
    spriteFilename(spriteFilename),
    spriteGenerated(false),
    isAlive(true), // 默认设置为存活
    drop(drop){} 

// 默认构造函数
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE,2.0f),// 默认攻击范围为2
    aggressionLevel(1),
    detectionRadius(10.0f),
    baseLevel(1),
    spriteFilename(""),
    spriteGenerated(false),
    isAlive(true), // 默认为存活
    drop(0){} // 默认设置为无掉落

// 获取攻击性等级
int Enemy::getAggressionLevel() const {
    return aggressionLevel;
}

// 获取索敌半径
float Enemy::getDetectionRadius() const {
    return detectionRadius;
}

// 获取基准等级
int Enemy::getBaseLevel() const {
    return baseLevel;
}

// 获取精灵文件名
std::string Enemy::getSpriteFilename() const {
    return spriteFilename;
}

// 受到伤害
void Enemy::takeDamage(float amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
        setIsAlive(false);
    }
    currentCooldown = 0.5; // 受击的攻击僵直
}

// 获取存活状态
bool Enemy::getIsAlive() const {
    return isAlive;
}

// 设置存活状态
void Enemy::setIsAlive(bool alive) {
    isAlive = alive;
}

// 攻击敌人
void Enemy::attackTarget(Entities& target, float amount=0, Element element1= Element::FIRE) {
    // 使用基类的攻击范围检查
    if (attackInRange(target)) {
        float elementModifier = calculateElementalDamageModifier(element, target.getElement());
        target.takeDamage(elementModifier * attack * aggressionLevel);
        currentCooldown = 5.0f / aggressionLevel;
    }
    else {
        CCLOG("Target is out of range.");
    }
}

// 敌人AI行为
void Enemy::aiBehavior(float distance, Player* player) {
    if (!isAlive) {
        // 如有掉落物则掉落并且重设掉落内容为0
        if (drop) {
            player->addItemToBackpack(drop, 1);
            drop = 0;
        }
        return;
    }
    
    // 不在索敌区间内则待机
    if (distance > detectionRadius) {
        return;
    }

    // 计算敌人和玩家之间的向量
    cocos2d::Vec2 enemyPosition = getPosition();
    cocos2d::Vec2 playerPosition = player->getPosition();
    cocos2d::Vec2 directionToPlayer = playerPosition - enemyPosition;

    // 如果距离玩家小于攻击范围，敌人发动攻击
    if (distance < attackRange) {
        if (canAttack()) {
            attackTarget(*player);  // 假设有攻击函数
            CCLOG("Enemy attacks player, distance: %.2f", distance);
        }
    }
    // 如果玩家等级高于敌人，敌人可能会做出逃避行为
    else if (player->getLevel() > baseLevel * 2) {
        CCLOG("Enemy is fleeing, player's level is too high.");
        // 逃跑逻辑
        // 计算逃跑方向：逃跑方向就是玩家位置的反向
        cocos2d::Vec2 fleeDirection = -directionToPlayer.getNormalized();  // 反向，单位化

        // 移动敌人
        float fleeSpeed = 200.0f; // 逃跑速度
        setPosition(enemyPosition + fleeDirection * fleeSpeed * 0.1f);  // 每帧按速度移动
    }
    else {
        CCLOG("Enemy is pursuing player, distance: %.2f", distance);
        // 追击逻辑，敌人继续朝玩家移动
        // 计算追击方向
        cocos2d::Vec2 pursuitDirection = directionToPlayer.getNormalized();  // 单位化
         // 移动敌人
        float pursuitSpeed = 100.0f * aggressionLevel; // 追击速度
        setPosition(enemyPosition + pursuitDirection * pursuitSpeed * 0.1f);  // 每帧按速度移动
    }
}

// 打印敌人状态
void Enemy::printStatus() {
    Entities::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Attack Range: %.2f", attackRange); // 显示攻击范围
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Sprite Filename: %s", spriteFilename.c_str());
    CCLOG("Is Alive: %s", isAlive ? "Yes" : "No");  // 打印存活状态
}

// 将敌人对象序列化为 JSON 字符串
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
    doc.AddMember("attackRange", attackRange, allocator); // 添加攻击范围
    doc.AddMember("baseLevel", baseLevel, allocator);
    doc.AddMember("spriteFilename", rapidjson::Value(spriteFilename.c_str(), allocator), allocator);
    doc.AddMember("isAlive", isAlive, allocator); // 保存存活状态

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串加载数据
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
    if (doc.HasMember("attackRange")) attackRange = doc["attackRange"].GetFloat();  // 加载攻击范围
    if (doc.HasMember("baseLevel")) baseLevel = doc["baseLevel"].GetInt();
    if (doc.HasMember("spriteFilename")) spriteFilename = doc["spriteFilename"].GetString();
    if (doc.HasMember("isAlive")) isAlive = doc["isAlive"].GetBool(); // 加载存活状态
}

// 带有新位置的clone
Enemy* Enemy::clone(const cocos2d::Vec2& newPosition) {
    Enemy* newEnemy = new Enemy(health, attack, defence, element, aggressionLevel, detectionRadius, attackRange, baseLevel, spriteFilename, drop);
    newEnemy->setPosition(newPosition);
    return newEnemy;
}

// 根据文件名生成精灵
cocos2d::Sprite* Enemy::generateSprite() {
    if (!spriteGenerated) {
        auto sprite = cocos2d::Sprite::create(spriteFilename);
        if (sprite) {
            spriteGenerated = true;  // 精灵生成成功后标记为 true
            return sprite;
        }
    }
    return nullptr;  // 如果精灵已经生成或失败，返回空指针
}
