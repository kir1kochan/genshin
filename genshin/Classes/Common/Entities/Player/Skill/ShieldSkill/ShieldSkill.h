#ifndef SHIELDSKILL_H
#define SHIELDSKILL_H

#include "Skill.h"

class ShieldSkill : public Skill {
private:
    float shieldValue;   // 初始护甲值


public:
    ShieldSkill(const std::string& name, float cooldown);

    // 激活护甲技能
    void activate(Entities* user, Entities& target) override;
};

#endif // SHIELDSKILL_H