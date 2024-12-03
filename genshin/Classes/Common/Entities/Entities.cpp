#include "Entities.h"

// 构造函数
Entities::Entities(float health, float attack, float defence, Element element,float attackRange)
    : health(health), maxHealth(health), attack(attack), defence(defence), element(element), attackRange(attackRange) {}


// 默认构造函数
Entities::Entities()
    : health(100), maxHealth(100), attack(10), defence(2), element(Element::FIRE), attackRange(1.5f) {}

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
    statusEffects.erase(effect);
}

// 更新状态效果
void Entities::updateStatusEffects(float deltaTime) {
    // 更新状态持续时间
    for (auto itr = statusEffects.begin(); itr != statusEffects.end();) {
        itr->second -= deltaTime;  // 减少持续时间
        if (itr->second <= 0) {
            // 如果状态已过期，移除该状态
            itr = statusEffects.erase(itr);
        }
        else {
            ++itr;  // 如果状态仍在持续，继续检查下一个状态
        }
    }
}