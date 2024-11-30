#ifndef SHIELDSKILL_H
#define SHIELDSKILL_H

#include "Skill.h"

class ShieldSkill : public Skill {
private:
    float shieldValue;   // ��ʼ����ֵ


public:
    ShieldSkill(int id,const std::string& name, float cooldown, float shieldValue);

    // ����׼���
    void activate(Player* user, Enemy& target) override;
};

#endif // SHIELDSKILL_H