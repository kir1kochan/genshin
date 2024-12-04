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
    std::string name;       // ��������
    float cooldown;         // ������ȴʱ�䣨�룩
    float currentCooldown;  // ��ǰʣ����ȴʱ��

public:
    // ���캯��
    Skill(int id, const std::string& name, float cooldown);
    virtual ~Skill() = default;

    // ��ȡ��������
    std::string getSkillName() const;
    int getId() const;
    bool isOnCooldown() const;       // ��鼼���Ƿ�����ȴ
    void updateCooldown(float deltaTime);  // ������ȴʱ��
    void resetCooldown();            // ������ȴ

    virtual void activate(Player* user, Enemy& target) = 0;

    // ���ݼ���ID���Ҽ���
    static std::shared_ptr<Skill> findById(int skillId);

};

#endif // SKILL_H