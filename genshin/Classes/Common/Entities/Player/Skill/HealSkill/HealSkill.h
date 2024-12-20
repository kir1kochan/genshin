#ifndef HEAL_SKILL_H
#define HEAL_SKILL_H

#include "../Skill.h"

class Player;
class Enemy;

class HealSkill : public Skill {
private:
    float healAmount; // 恢复的生命值

public:
    HealSkill(int id, const std::string& name, float cooldown, float staminaCost,float healAmount);

    float getHealAmount() const{
        return healAmount;
    }

    // 激活治疗技能
    void activate(Player* user, Enemy& target) override;

    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString) override;
};

#endif // HEAL_SKILL_H