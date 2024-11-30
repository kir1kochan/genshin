#ifndef HEAL_SKILL_H
#define HEAL_SKILL_H

#include "Skill.h"

class HealSkill : public Skill {
private:
    float healAmount; // 恢复的生命值

public:
    HealSkill(const std::string& name, float cooldown, float healAmount);

    // 激活治疗技能
    void activate(Entities* user, Entities& target) override;
};

#endif // HEAL_SKILL_H