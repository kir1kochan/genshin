#include "Skill.h"
#include "cocos2d.h"
#include "../Player.h"
#include "../../Enemy/Enemy.h"
// ���캯��
Skill::Skill(int id, const std::string& name, float cooldown,float staminaCost)
    :id(id), name(name), cooldown(cooldown), currentCooldown(0.0f), staminaCost(staminaCost) {}
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

    // ���� JSON �ַ����������¼�ֵ��: id, name, cooldown, staminaCost
    if (doc.HasMember("id") && doc["id"].IsInt()) {
        id = doc["id"].GetInt();
    }

    if (doc.HasMember("name") && doc["name"].IsString()) {
        name = doc["name"].GetString();
    }

    if (doc.HasMember("cooldown") && doc["cooldown"].IsFloat()) {
        cooldown = doc["cooldown"].GetFloat();
    }

    // ��ʼ����ǰ��ȴʱ��Ϊ�����ȴʱ��
    currentCooldown = cooldown;

    // ��� staminaCost �Ƿ��� JSON ��
    if (doc.HasMember("staminaCost") && doc["staminaCost"].IsFloat()) {
        staminaCost = doc["staminaCost"].GetFloat();
    }
    else {
        // ���û���ṩ staminaCost����������ΪĬ��ֵ���� 0��
        staminaCost = 0.0f;
    }
}

bool Skill::canUse(Player* user, Enemy& target)
{
    // ����Ƿ�����ȴ
    if (isOnCooldown()) {
        CCLOG("Skill %s is on cooldown.", name.c_str());
        return false;
    }

    // ��������Ƿ��㹻
    if (user->getStamina() < staminaCost) {
        CCLOG("Not enough stamina for skill %s.", name.c_str());
        return false;
    }
    return true;  // �����������������ܿ���ʹ��
}

