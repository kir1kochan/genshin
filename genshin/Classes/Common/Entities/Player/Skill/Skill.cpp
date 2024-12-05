#include "Skill.h"
#include "cocos2d.h"
#include "../Player.h"
#include "../../Enemy/Enemy.h"
// 构造函数
Skill::Skill(int id, const std::string& name, float cooldown)
    : id(id), name(name), cooldown(cooldown), currentCooldown(0.0f) {}

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

// 从 JSON 字符串加载数据
void Skill::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Skill");
        return;
    }

    // 假设 JSON 字符串包含以下键值对: id, name, cooldown
    if (doc.HasMember("id") && doc["id"].IsInt()) {
        id = doc["id"].GetInt();
    }

    if (doc.HasMember("name") && doc["name"].IsString()) {
        name = doc["name"].GetString();
    }

    if (doc.HasMember("cooldown") && doc["cooldown"].IsFloat()) {
        cooldown = doc["cooldown"].GetFloat();
    }

    currentCooldown = cooldown;  // 可以选择将初始冷却时间设置为最大值
}