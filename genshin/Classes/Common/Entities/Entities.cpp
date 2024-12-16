#include "Entities.h"

// 构造函数
Entities::Entities(float health, float attack, float defence, Element element,float attackRange)
    : health(health), maxHealth(health), attack(attack), defence(defence), element(element), attackRange(attackRange),baseAttack(attack) {}


// 默认构造函数
Entities::Entities()
    : health(100), maxHealth(100), attack(10), defence(2), element(Element::FIRE), attackRange(1.5f), baseAttack(10) {}

// 虚析构函数
Entities::~Entities() {}

// 获取当前血量
float Entities::getHealth() const {
    return health;
}

// 获取最大血量
float Entities::getMaxHealth() const {
    return maxHealth;
}

// 获取元素类型
Element Entities::getElement() const {
    return element;
}

// 受到伤害
void Entities::takeDamage(float amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// 恢复血量
void Entities::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// 对目标造成伤害
void Entities::attackTarget(Entities& target, float amount, Element element) {
    float elementModifier = calculateElementalDamageModifier(element, target.getElement());
    target.takeDamage(elementModifier * amount);
}


// 打印当前状态
void Entities::printStatus() {
    CCLOG("Health: %.2f/%.2f", health, maxHealth);
    CCLOG("Attack: %.2f", attack);
    CCLOG("Defence: %.2f", defence);
    CCLOG("Element: %s", elementToString(element).c_str());
}

// 更新攻击冷却
void Entities::updateAttackCooldown(float deltaTime) {
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;  // 减少冷却时间
        if (currentCooldown < 0) {
            currentCooldown = 0;  // 确保冷却时间不会小于0
        }
    }
}

// 判断是否可以攻击
bool Entities::canAttack() const {
    return currentCooldown == 0;
}

float Entities::getAttackRange() const
{
    return attackRange;
}

bool Entities::attackInRange(Entities& target)
{
    cocos2d::Vec2 distanceVec = target.getPosition() - this->getPosition();
    float distance = distanceVec.length();
    return distance <= attackRange;
}

// 每帧更新逻辑
void Entities::update(float deltaTime) {
    // 更新攻击冷却
    attackCooldownAccumulator += deltaTime;
    if (attackCooldownAccumulator >= attackCooldownInterval) {
        updateAttackCooldown(attackCooldownAccumulator);  // 更新攻击冷却
        attackCooldownAccumulator = 0.0f;  // 重置时间
    }

    // 更新状态效果
    updateStatusEffects(deltaTime);
}


// 将所有成员变量序列化为 JSON 格式，生成 JSON 字符串
std::string Entities::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 将数据加入 JSON 对象
    doc.AddMember("health", health, allocator);
    doc.AddMember("maxHealth", maxHealth, allocator);
    doc.AddMember("attack", attack, allocator);
    doc.AddMember("defence", defence, allocator);
    doc.AddMember("element", static_cast<int>(element), allocator);
    doc.AddMember("attackRange", attackRange, allocator); 

    // 转换为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串中读取成员变量序列
void Entities::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // 从 JSON 对象中提取数据
    if (doc.HasMember("health")) health = doc["health"].GetFloat();
    if (doc.HasMember("maxHealth")) maxHealth = doc["maxHealth"].GetFloat();
    if (doc.HasMember("attack")) attack = doc["attack"].GetFloat();
    if (doc.HasMember("defence")) defence = doc["defence"].GetFloat();
    if (doc.HasMember("element")) element = static_cast<Element>(doc["element"].GetInt());
    if (doc.HasMember("attackRange")) attackRange = doc["attackRange"].GetFloat();  
    baseAttack = attack;
}

// 将成员变量序列化为 JSON 格式，并保存到本地
void Entities::saveToFile(const std::string& filePath) const {
    std::string jsonString = saveToJson(); // 调用 saveToJson 获取 JSON 字符串
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonString;
        file.close();
        CCLOG("Entity saved to file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

// 从本地读取 JSON 文件，读取成员变量序列
void Entities::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf(); // 读取整个文件内容到 buffer
        file.close();
        std::string jsonString = buffer.str();
        loadFromJson(jsonString); // 调用 loadFromJson 解析 JSON 数据
        CCLOG("Entity loaded from file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}


// 添加状态效果
void Entities::applyStatusEffect(const std::string& effect, float duration) {
    statusEffects[effect] = duration;
}

// 移除状态效果
void Entities::removeStatusEffect(const std::string& effect) {
    statusEffects[effect] = 0.0f;
}

// 更新状态效果
void Entities::updateStatusEffects(float deltaTime) {
    for (auto& status : statusEffects) {
        if (status.second > 0.0f) {
            status.second -= deltaTime;  // 减少状态持续时间

            // 检查每种状态并触发相应的效果
            if (status.first == "Burning" && status.second > 0.0f) {
                // 燃烧状态：每秒造成5点伤害
                takeDamage(5.0f);
                CCLOG("Burning effect deals 5 damage.");
            }
            else if (status.first == "Frozen" && status.second > 0.0f) {
                // 冰冻状态：目标无法行动
                CCLOG("Frozen: Target is immobilized.");
                // 这里可以加上额外逻辑，如冰冻状态下无法移动、攻击等
                // 需要与后续移动联动，暂且不加入
            }
            else if (status.first == "Poisoned" && status.second > 0.0f) {
                // 中毒状态：每秒造成2点伤害
                takeDamage(3.0f);
                CCLOG("Poisoned effect deals 2 damage.");
            }
            else if (status.first == "Paralyzed" && status.second > 0.0f) {
                // 麻痹状态：无法行动
                CCLOG("Paralyzed: Target cannot act.");
                // 这里可以加上麻痹状态下无法攻击、无法行动等逻辑
                // 需要与后续移动联动，暂且不加入
            }
            else if (status.first == "Weakened" && status.second > 0.0f) {
                // 设置虚弱状态下的攻击力减少
                attack = baseAttack * 0.5f;  // 攻击力减少50%
                CCLOG("Weakened: Attack is reduced to %.2f.", attack);
            }
            else if (status.first == "Drenched" && status.second > 0.0f) {
                // 湿润状态：提高雷电技能伤害等
                CCLOG("Drenched: Target is wet.");
            }
        }

        // 当状态时间为0或小于0时，表示状态效果已结束，可以在此处做处理
        if (status.second <= 0.0f) {
            if (status.first == "Weakened") {
                // 虚弱状态结束时恢复攻击力
                attack = baseAttack;  // 恢复原始攻击力
                CCLOG("Weakened effect expired: Attack restored to %.2f.", attack);
            }
            CCLOG("%s effect expired.", status.first.c_str());
            // 不删除状态，只是继续处理时间结束后的效果
            // 可以选择在这里添加一些额外的逻辑，比如解除状态后触发其他效果，比如虚弱状态恢复
        }
    }
}

bool Entities::hasStatusEffect(const std::string& effect) const {
    auto it = statusEffects.find(effect);
    return it != statusEffects.end() && it->second > 0.0f;
}


void Entities::applyElementalEffects(Entities& target, Element skillElement) {
    if (target.getElement() == skillElement)    //若技能元素与敌方元素一致，则无法起效
        return;
    switch (skillElement) {
        // 火系技能：使目标燃烧
    case Element::FIRE:
        if (!target.hasStatusEffect("Burning")) {
            target.applyStatusEffect("Burning", 5.0f);  // 火焰持续5秒
            CCLOG("Target is now Burning for 5 seconds.");
        }
        // 火系技能对冰冻目标额外伤害并解除冻结
        if (target.hasStatusEffect("Frozen")) {
            target.removeStatusEffect("Frozen");
            target.takeDamage(15.0f);  // 解除冰冻后额外伤害
            CCLOG("Fire skill melts frozen target and deals extra damage.");
        }
        break;

        // 冰系技能：使目标冻结
    case Element::ICE:
        if (!target.hasStatusEffect("Frozen")) {
            target.applyStatusEffect("Frozen", 3.0f);  // 冰冻持续3秒
            CCLOG("Target is now Frozen for 3 seconds.");
        }
        // 冰系技能对湿润目标增强冻结效果
        if (target.hasStatusEffect("Drenched")) {
            target.applyStatusEffect("Frozen", 6.0f);  // 冰冻效果加长
            CCLOG("Ice skill enhances freeze effect on Drenched target.");
        }
        break;

        // 水系技能：使目标湿润
    case Element::WATER:
        if (!target.hasStatusEffect("Drenched")) {
            target.applyStatusEffect("Drenched", 5.0f);  // 湿润状态持续5秒
            CCLOG("Target is now Drenched for 5 seconds.");
        }
        // 水系技能对火系目标造成额外伤害
        if (target.getElement() == Element::FIRE) {
            target.takeDamage(10.0f);  // 水系技能对火系目标造成额外伤害
            CCLOG("Water skill deals extra damage to Fire target.");
        }
        break;

        // 雷系技能：对湿润目标造成额外伤害
    case Element::THUNDER:
        if (target.hasStatusEffect("Drenched")) {
            target.takeDamage(20.0f);  // 雷电对湿润目标额外伤害
            CCLOG("Thunder skill deals extra damage to Drenched target.");
        }
        // 雷系技能可能造成麻痹状态
        if (!target.hasStatusEffect("Paralyzed")) {
            target.applyStatusEffect("Paralyzed", 2.0f);  // 麻痹效果持续2秒
            CCLOG("Target is now Paralyzed for 2 seconds.");
        }
        break;

        // 风系技能：增加火焰伤害
    case Element::AIR:
        if (target.hasStatusEffect("Burning")) {
            target.takeDamage(5.0f);  // 火焰持续时间延长或增加额外伤害
            target.applyStatusEffect("Burning", 2.0f);  // 火焰持续2秒
            CCLOG("Wind skill increases damage on Burning target.");
        }
        break;

        // 土系技能：减少敌人防御
    case Element::EARTH:
        if (!target.hasStatusEffect("Weakened")) {
            target.applyStatusEffect("Weakened", 5.0f);  // 减少目标的防御
            CCLOG("Target is now Weakened for 5 seconds.");
        }
        break;

        // 草系技能：使目标进入“中毒”状态
    case Element::GRASS:
        if (!target.hasStatusEffect("Poisoned")) {
            target.applyStatusEffect("Poisoned", 8.0f);  // 中毒持续5秒
            CCLOG("Target is now Poisoned for 5 seconds.");
        }
        break;

    default:
        break;
    }
}