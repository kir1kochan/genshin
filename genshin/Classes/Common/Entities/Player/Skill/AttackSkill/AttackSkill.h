#ifndef ATTACK_SKILL_H
#define ATTACK_SKILL_H

#include "Skill.h"
#include "Player.h"
#include "Element.h"

class AttackSkill : public Skill {
private:
    float attackPower; // 技能攻击力
    float range;       // 技能范围
    Element element

public:
    AttackSkill(int id, const std::string& name, float cooldown, float attackPower, float range, Element element);

    // 获取攻击力与范围
    float getAttackPower() const;
    float getRange() const;

    // 实现技能激活
    void activate(Entities* user, Entities& target) override;
};

#endif // ATTACK_SKILL_H
