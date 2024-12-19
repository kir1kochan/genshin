#include "Skill.h"
#include "cocos2d.h"
#include "../Player.h"
#include "../../Enemy/Enemy.h"
// 构造函数
Skill::Skill(int id, const std::string& name, float cooldown,float staminaCost)
    :id(id), name(name), cooldown(cooldown), currentCooldown(0.0f), staminaCost(staminaCost) {}
// 获取技能名称
std::string Skill::getSkillName() const {
    return name;
}

int Skill::getId() const {
    return id;
}

// 检查技能是否在冷却
bool Skill::isOnCooldown() const {
    return currentCooldown > 0.0f;
}

// 更新冷却时间
void Skill::updateCooldown(float deltaTime) {
    if (currentCooldown > 0.0f) {
        currentCooldown -= deltaTime;
        if (currentCooldown < 0.0f) {
            currentCooldown = 0.0f;  // 确保冷却时间不为负数
        }
    }
}

// 重置冷却
void Skill::resetCooldown() {
    currentCooldown = cooldown;
}

float Skill::getStaminaCost() const
{
    return staminaCost;
}

void Skill::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Skill");
        return;
    }

    // 假设 JSON 字符串包含以下键值对: id, name, cooldown, staminaCost
    if (doc.HasMember("id") && doc["id"].IsInt()) {
        id = doc["id"].GetInt();
    }

    if (doc.HasMember("name") && doc["name"].IsString()) {
        name = doc["name"].GetString();
    }

    if (doc.HasMember("cooldown") && doc["cooldown"].IsFloat()) {
        cooldown = doc["cooldown"].GetFloat();
    }

    // 初始化当前冷却时间为最大冷却时间
    currentCooldown = cooldown;

    // 检查 staminaCost 是否在 JSON 中
    if (doc.HasMember("staminaCost") && doc["staminaCost"].IsFloat()) {
        staminaCost = doc["staminaCost"].GetFloat();
    }
    else {
        // 如果没有提供 staminaCost，可以设置为默认值（如 0）
        staminaCost = 0.0f;
    }
}

bool Skill::canUse(Player* user, Enemy& target)
{
    // 检查是否在冷却
    if (isOnCooldown()) {
        CCLOG("Skill %s is on cooldown.", name.c_str());
        return false;
    }

    // 检查体力是否足够
    if (user->getStamina() < staminaCost) {
        CCLOG("Not enough stamina for skill %s.", name.c_str());
        return false;
    }
    return true;  // 满足所有条件，技能可以使用
}

