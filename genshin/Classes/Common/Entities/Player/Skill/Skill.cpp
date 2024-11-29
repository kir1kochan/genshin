#include "Skill.h"
#include "cocos2d.h"

// ���캯��
Skill::Skill(const std::string& name, float cooldown)
    : name(name), cooldown(cooldown), currentCooldown(0.0f) {}

// ��ȡ��������
std::string Skill::getName() const {
    return name;
}

// ��鼼���Ƿ�����ȴ
bool Skill::isOnCooldown() const {
    return currentCooldown > 0.0f;
}

// ������ȴʱ��
void Skill::updateCooldown(float deltaTime) {
    if (currentCooldown > 0.0f) {
        currentCooldown -= deltaTime;
        if (currentCooldown < 0.0f) {
            currentCooldown = 0.0f;  // ȷ����ȴʱ�䲻Ϊ����
        }
    }
}

// ������ȴ
void Skill::resetCooldown() {
    currentCooldown = cooldown;
}