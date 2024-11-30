#include "Skill.h"
#include "cocos2d.h"

// 构造函数
Skill::Skill(int id, const std::string& name, float cooldown)
    : id(id), name(name), cooldown(cooldown), currentCooldown(0.0f) {}

// 获取技能名称
std::string Skill::getName() const {
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