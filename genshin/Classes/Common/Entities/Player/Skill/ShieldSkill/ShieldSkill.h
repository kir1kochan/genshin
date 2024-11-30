#ifndef SHIELDSKILL_H
#define SHIELDSKILL_H

#include "Skill.h"

class ShieldSkill : public Skill {
private:
    float shieldValue;   // 初始护甲值


public:
    ShieldSkill(int id,const std::string& name, float cooldown, float shieldValue);

    // 激活护甲技能
    void activate(Player* user, Enemy& target) override;
};

#endif // SHIELDSKILL_H