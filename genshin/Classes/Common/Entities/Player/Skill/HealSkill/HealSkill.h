#ifndef HEAL_SKILL_H
#define HEAL_SKILL_H

#include "../Skill.h"

class Player;
class Enemy;

class HealSkill : public Skill {
private:
    float healAmount; // �ָ�������ֵ

public:
    HealSkill(int id, const std::string& name, float cooldown, float staminaCost,float healAmount);

    float getHealAmount() const{
        return healAmount;
    }

    // �������Ƽ���
    void activate(Player* user, Enemy& target) override;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;
};

#endif // HEAL_SKILL_H