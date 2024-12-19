#ifndef ATTACK_SKILL_H
#define ATTACK_SKILL_H

#include "../Skill.h"
//#include "../../Player.h"
#include "Classes/Common/Element/Element.h"

class Player;
class Enemy;

class AttackSkill : public Skill {
private:
    float attackPower; // 技能攻击力
    float range;       // 技能范围
    Element element;

public:
    AttackSkill(int id, const std::string& name, float cooldown, float staminaCost, float attackPower, float range, Element element);
    
    // 获取攻击力与范围
    float getAttackPower() const;
    float getRange() const;

    // 实现技能激活
    void activate(Player* user, Enemy& target) override;


    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString) override;

    //判断能否使用技能
    bool canUse(Player* user, Enemy& target) override;

};

#endif // ATTACK_SKILL_H
