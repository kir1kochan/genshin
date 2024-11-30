#ifndef ATTACK_SKILL_H
#define ATTACK_SKILL_H

#include "Skill.h"

class AttackSkill : public Skill {
private:
    float attackPower; // ���ܹ�����
    float range;       // ���ܷ�Χ

public:
    AttackSkill(const std::string& name, float cooldown, float attackPower, float range);

    // ��ȡ�������뷶Χ
    float getAttackPower() const;
    float getRange() const;

    // ʵ�ּ��ܼ���
    void activate(Entities* user, Entities& target) override;
};

#endif // ATTACK_SKILL_H
