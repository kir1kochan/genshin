#ifndef SHIELDSKILL_H
#define SHIELDSKILL_H

#include "../Skill.h"

class Player;
class Enemy;

class ShieldSkill : public Skill {
private:
    float shieldValue;   // ��ʼ����ֵ
    float shieldTime;    //���׵ĳ���


public:
    ShieldSkill(int id,const std::string& name, float cooldown, float staminaCost, float shieldValue,float shieldTime);

    // ����׼���
    void activate(Player* user, Enemy& target) override;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;
};

#endif // SHIELDSKILL_H