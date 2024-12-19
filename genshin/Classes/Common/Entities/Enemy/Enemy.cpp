#include "Enemy.h"
#include "../Player/Player.h"
#include <vector>

// 构造函数
Enemy::Enemy(float health, float attack, float defence, Element element, float attackRange, int aggressionLevel, float detectionRadius,
     int baseLevel, const std::string& imagePath, int drop)
    : Entities(health, attack, defence, element, attackRange),  
    aggressionLevel(aggressionLevel),
    detectionRadius(detectionRadius),
    baseLevel(baseLevel),
    imagePath(imagePath),
    spriteGenerated(false),
    isAlive(true), // 默认设置为存活
    drop(drop){} 

// 默认构造函数
Enemy::Enemy()
    : Entities(100, 10, 2, Element::FIRE, 30.0f),// 默认攻击范围为2
    aggressionLevel(1),
    detectionRadius(100.0f),
    baseLevel(1),
    imagePath(""),
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
    return imagePath;
}

// 受到伤害
void Enemy::takeDamage(float amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
        setIsAlive(false);
        sprite->stopActionByTag(100);

        // 创建闪烁动作：2秒内闪烁5次
        auto blinkAction = Blink::create(1.0f, 5);

        // 闪烁完成后的回调：让怪物消失
        auto disappearCallback = CallFunc::create([this]() {
            // 将精灵从父节点移除
            sprite->removeFromParent();
            this->removeFromParent(); // 移除敌人节点本身
            CCLOG("Enemy has died.");
            });

        // 组合动作：闪烁 -> 消失
        auto deathSequence = Sequence::create(blinkAction, disappearCallback, nullptr);

        // 运行动作
        sprite->runAction(deathSequence);

    }

    drawProgressBar(barNode, health / maxHealth);
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
        if (baseLevel) {
            player->gainExperience(30 + 2 * baseLevel);
            player->getBackpack()->addCoins(30 + 2 * baseLevel);
            baseLevel = 0;
        }
        return;
    }
    
    // 不在索敌区间内则待机
    if (distance > detectionRadius) {
        return;
    }
    //  如果此时处于麻痹或者冰冻状态，则无法移动和攻击
    if (this->hasStatusEffect("Frozen") || this->hasStatusEffect("Paralyzed")) {
        return;
    }
    bool isAttacking = false;
    // 计算敌人和玩家之间的向量
    cocos2d::Vec2 enemyPosition = getPosition();
    cocos2d::Vec2 playerPosition = player->getPosition();
    cocos2d::Vec2 directionToPlayer = playerPosition - enemyPosition;

    // 计算敌人朝向（基于方向）
    cocos2d::Vec2 normalizedDirection = directionToPlayer.getNormalized();
    float angle = normalizedDirection.getAngle();  // 获取角度（弧度）

    // 如果距离玩家小于攻击范围，敌人发动攻击
    if (distance < attackRange) {
        if (canAttack()) {
            attackTarget(*player);  // 假设有攻击函数
            CCLOG("Enemy attacks player, distance: %.2f", distance);
            isAttacking = true;
            // 停止当前动画
            sprite->stopActionByTag(100);

            // 执行攻击动画（只播放一次）
            auto attackAnimate = getDirectionAnimate(angle, isAttacking);
            auto resetToNormal = CallFunc::create([this, angle]() {
                auto repeatNormalAnimate = RepeatForever::create(getDirectionAnimate(angle, false));
                repeatNormalAnimate->setTag(100); // 给动作设置Tag
                sprite->runAction(repeatNormalAnimate); // 重新开始移动动画
                });

            // 使用 Sequence 动作链：攻击动画 + 重置动画
            auto attackSequence = Sequence::create(attackAnimate, resetToNormal, nullptr);
            sprite->runAction(attackSequence);
            return;
        }
    }
    // 如果玩家等级高于敌人，敌人可能会做出逃避行为
    else if (player->getLevel() > baseLevel * 2) {
        CCLOG("Enemy is fleeing, player's level is too high.");
        // 逃跑逻辑
        // 计算逃跑方向：逃跑方向就是玩家位置的反向
        cocos2d::Vec2 fleeDirection = -directionToPlayer.getNormalized();  // 反向，单位化
        angle *= -1;
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
    auto repeatAnimate = RepeatForever::create(getDirectionAnimate(angle,isAttacking));
    repeatAnimate->setTag(100); // 方便后续停止
    sprite->stopActionByTag(100);
    sprite->runAction(repeatAnimate);
}

// 打印敌人状态
void Enemy::printStatus() {
    Entities::printStatus();
    CCLOG("Aggression Level: %d", aggressionLevel);
    CCLOG("Detection Radius: %.2f", detectionRadius);
    CCLOG("Attack Range: %.2f", attackRange); // 显示攻击范围
    CCLOG("Base Level: %d", baseLevel);
    CCLOG("Sprite Filename: %s", imagePath.c_str());
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
    doc.AddMember("imagePath", rapidjson::Value(imagePath.c_str(), allocator), allocator);
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
    if (doc.HasMember("imagePath")) imagePath = doc["imagePath"].GetString();
    if (doc.HasMember("isAlive")) isAlive = doc["isAlive"].GetBool(); // 加载存活状态
}

// 带有新位置的clone
Enemy* Enemy::clone(const cocos2d::Vec2& newPosition) {
    Enemy* newEnemy = new Enemy(health, attack, defence, element, aggressionLevel, detectionRadius, attackRange, baseLevel, imagePath, drop);
    newEnemy->setPosition(newPosition);
    return newEnemy;
}

// 根据文件名生成精灵
cocos2d::Sprite* Enemy::generateSprite() {
    if (!spriteGenerated) {
        // 加载纹理
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(imagePath);

        // 每帧的尺寸
        int frameWidth = 64;
        int frameHeight = 64;


        // 动画方向和动作设置 (4个方向，2种状态：正常、攻击)
        const int rows = 4;  // 4个方向
        const int cols = 6;  // 每个方向6帧 (前三帧为普通动作，后三帧为攻击动作)


        // 裁剪四个方向的动画帧
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < 3; ++col) { // 前三帧是普通动作
                Rect rect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
                auto frame = SpriteFrame::createWithTexture(texture, rect);
                frames.pushBack(frame);
            }
            for (int col = 3; col < 6; ++col) { // 后三帧是攻击动作
                Rect rect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
                auto frame = SpriteFrame::createWithTexture(texture, rect);
                attackFrames.pushBack(frame); // 存储攻击帧
            }
        }

        // 创建精灵并设置默认帧
        sprite = Sprite::createWithSpriteFrame(frames.front());
        this->addChild(sprite);

        for (int i = 0; i < 4; ++i) {
            Vector<SpriteFrame*> directionFrames;
            for (int j = i * 3; j < (i + 1) * 3; ++j) {
                directionFrames.pushBack(frames.at(j)); // 普通动作帧
            }
            auto animation = Animation::createWithSpriteFrames(directionFrames, 0.2f); // 0.2秒每帧
            auto animate = Animate::create(animation); // 创建 Animate    
            animates.pushBack(animate);
        }

        for (int i = 0; i < 4; ++i) {
            Vector<SpriteFrame*> directionAttackFrames;
            for (int j = i * 3; j < (i + 1) * 3; ++j) {
                directionAttackFrames.pushBack(attackFrames.at(j)); // 攻击动作帧
            }
            auto attackAnimation = Animation::createWithSpriteFrames(directionAttackFrames, 0.2f); // 0.2秒每帧
            auto animate = Animate::create(attackAnimation); // 创建 Animate
            animates.pushBack(animate);
        }

        auto defaultAnimate = RepeatForever::create(animates.at(2));
        defaultAnimate->setTag(100); // 给动作设置Tag
        // 根据敌人朝向切换动画
        sprite->runAction(defaultAnimate); // 默认朝下
        applyElementColor();
        spriteGenerated = true;
        barNode = DrawNode::create();
        this->addChild(barNode);
        barNode->setPosition(-20, 20);
        drawProgressBar(barNode, health / maxHealth);
        return sprite;
    }
    return nullptr; // 如果精灵已经生成，返回空指针
}

void Enemy::applyElementColor() {
    // 根据敌人的属性来设置颜色
     // 根据敌人的属性来设置颜色
    switch (element) {
    case Element::FIRE:
        this->sprite->setColor(cocos2d::Color3B::RED);
        break;
    case Element::WATER:
        this->sprite->setColor(cocos2d::Color3B::BLUE);
        break;
    case Element::EARTH:
        this->sprite->setColor(cocos2d::Color3B(139, 69, 19)); // 土黄色
        break;
    case Element::ICE:
        this->sprite->setColor(cocos2d::Color3B::WHITE);
        break;
    case Element::THUNDER:
        this->sprite->setColor(cocos2d::Color3B::YELLOW);
        break;
    case Element::AIR:
        this->sprite->setColor(cocos2d::Color3B(144, 238, 144)); // 浅绿色
        break;
    case Element::GRASS:
        this->sprite->setColor(cocos2d::Color3B(34, 139, 34)); // 深绿色
        break;
    default:
        this->sprite->setColor(cocos2d::Color3B::WHITE);
        break;
    }
}

cocos2d::Animate* Enemy::getDirectionAnimate(float angle, bool isAttacking) {
    if (angle >= -M_PI_4 && angle < M_PI_4) {
        return animates.at(isAttacking ? 7 : 3); // 攻击朝右 or 普通朝右
    }
    else if (angle >= M_PI_4 && angle < 3 * M_PI_4) {
        return animates.at(isAttacking ? 4 : 0); // 攻击朝上 or 普通朝上
    }
    else if (angle >= -3 * M_PI_4 && angle < -M_PI_4) {
        return animates.at(isAttacking ? 6 : 2); // 攻击朝下 or 普通朝下
    }
    else {
        return animates.at(isAttacking ? 5 : 1); // 攻击朝左 or 普通朝左
    }
}

void Enemy::setSpriteFilename(std::string filename) {
    imagePath = filename;
}

void Enemy::drawProgressBar(cocos2d::DrawNode* barNode, float percent) {
    barNode->clear(); // 清空旧的内容

    // 确保百分比在 0 到 1 之间
    percent = std::max(0.0f, std::min(1.0f, percent));
    float filledWidth = 40 * percent;

    // 绘制矩形进度条
    barNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(filledWidth, 5), Color4F::RED);

    // 绘制边框
    cocos2d::Vec2 origin(0, 0); // 起点
    cocos2d::Vec2 topRight(40, 5); // 右上角点
    barNode->drawRect(origin, topRight, cocos2d::Color4F::BLACK); // 黑色边框
}
