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

// �� JSON �ַ�����������
void Skill::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for Skill");
        return;
    }

    // ���� JSON �ַ����������¼�ֵ��: id, name, cooldown
    if (doc.HasMember("id") && doc["id"].IsInt()) {
        id = doc["id"].GetInt();
    }

    if (doc.HasMember("name") && doc["name"].IsString()) {
        name = doc["name"].GetString();
    }

    if (doc.HasMember("cooldown") && doc["cooldown"].IsFloat()) {
        cooldown = doc["cooldown"].GetFloat();
    }

    currentCooldown = cooldown;  // ����ѡ�񽫳�ʼ��ȴʱ������Ϊ���ֵ
}