#include "Skill.h"
#include "cocos2d.h"
#include "../Player.h"
#include "../../Enemy/Enemy.h"
// ���캯��
Skill::Skill(int id, const std::string& name, float cooldown)
    : id(id), name(name), cooldown(cooldown), currentCooldown(0.0f) {}

// ��ȡ��������
std::string Skill::getSkillName() const {
    return name;
}

int Skill::getId() const {
    return id;
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

std::shared_ptr<Skill> Skill::findById(int skillId) {
    auto it = allSkills.find(skillId);
    if (it != allSkills.end()) {
        return it->second;  // �ҵ����ܣ�����
    }
    return nullptr;  // ���û���ҵ�������nullptr
}