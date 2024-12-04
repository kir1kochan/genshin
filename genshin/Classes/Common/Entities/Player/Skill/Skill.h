#ifndef SKILL_H
#define SKILL_H

#include <string>
//#include "../../Enemy/Enemy.h"
//#include "../Player.h"

class Player;
class Enemy;

class Skill : public cocos2d::Node {
protected:
    int id;
    std::string name;       // 技能名称
    float cooldown;         // 技能冷却时间（秒）
    float currentCooldown;  // 当前剩余冷却时间

public:
    // 构造函数
    Skill(int id, const std::string& name, float cooldown);
    virtual ~Skill() = default;

    // 获取技能名称
    std::string getSkillName() const;
    int getId() const;
    bool isOnCooldown() const;       // 检查技能是否在冷却
    void updateCooldown(float deltaTime);  // 更新冷却时间
    void resetCooldown();            // 重置冷却

    virtual void activate(Player* user, Enemy& target) = 0;

    // 根据技能ID查找技能
    static std::shared_ptr<Skill> findById(int skillId);

};

#endif // SKILL_H