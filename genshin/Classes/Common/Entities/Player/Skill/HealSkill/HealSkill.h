#ifndef HEAL_SKILL_H
#define HEAL_SKILL_H

#include "Skill.h"

class HealSkill : public Skill {
private:
    float healAmount; // �ָ�������ֵ

public:
    HealSkill(int id, const std::string& name, float cooldown, float healAmount);

    // �������Ƽ���
    void activate(Entities* user, Entities& target) override;
};

#endif // HEAL_SKILL_H