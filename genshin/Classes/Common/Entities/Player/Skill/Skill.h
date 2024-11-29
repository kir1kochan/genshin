#ifndef SKILL_H
#define SKILL_H

#include <string>
#include "../Entities.h"

class Skill {
protected:
    std::string name;       // ��������
    float cooldown;         // ������ȴʱ�䣨�룩
    float currentCooldown;  // ��ǰʣ����ȴʱ��

public:
    // ���캯��
    Skill(const std::string& name, float cooldown);
    virtual ~Skill() = default;

    // ��ȡ��������
    std::string getName() const;

    bool isOnCooldown() const;       // ��鼼���Ƿ�����ȴ
    void updateCooldown(float deltaTime);  // ������ȴʱ��
    void resetCooldown();            // ������ȴ

    virtual void activate(Entities* user, Entities& target) = 0;
};

#endif // SKILL_H