#ifndef ATTACK_SKILL_H
#define ATTACK_SKILL_H

#include "../Skill.h"
//#include "../../Player.h"
#include "Classes/Common/Element/Element.h"

class Player;
class Enemy;

class AttackSkill : public Skill {
private:
    float attackPower; // ���ܹ�����
    float range;       // ���ܷ�Χ
    Element element;

public:
    AttackSkill(int id, const std::string& name, float cooldown, float staminaCost, float attackPower, float range, Element element);
    
    // ��ȡ�������뷶Χ
    float getAttackPower() const;
    float getRange() const;

    // ʵ�ּ��ܼ���
    void activate(Player* user, Enemy& target) override;


    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;

    //�ж��ܷ�ʹ�ü���
    bool canUse(Player* user, Enemy& target) override;

};

#endif // ATTACK_SKILL_H
