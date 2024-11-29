#ifndef SHIELDSKILL_H
#define SHIELDSKILL_H

#include "Skill.h"

class ShieldSkill : public Skill {
private:
    float shieldValue;   // ��ʼ����ֵ


public:
    ShieldSkill(const std::string& name, float cooldown);

    // ����׼���
    void activate(Entities* user, Entities& target) override;
};

#endif // SHIELDSKILL_H