#ifndef PLAYERSPIRIT_H
#define PLAYERSPIRIT_H

#include "../Spirit.h"

class PlayerSpirit : public Spirit {
private:
    int experience;  // ��Ҿ���ֵ
    int level;       // ��ҵȼ�

public:
    PlayerSpirit(float health, Element element)
        : Spirit(health, element), experience(0), level(1) {}

    // ���ڲ��Ե�Ĭ�Ϲ��캯��
    PlayerSpirit() : Spirit(100, Element::WATER){}

    // ����
    void levelUp() {
        level++;
        maxHealth += 20.0f;  // ÿ�������������Ѫ��
        health = maxHealth;   // Ѫ���ָ�
        CCLOG("Level up! Now level %d", level);
    }

    // ��ȡ��ǰ�ȼ�
    int getLevel() const { return level; }

    // ��������
    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {  // ����100����ֵ����
            experience = 0;
            levelUp();
        }
    }

    // ������ҵ���ʱ����Ԫ�����
    void attack(Spirit& target) override {
        if (element == Element::FIRE && target.getElement() == Element::EARTH) {
            target.takeDamage(20.0f);  // �����
        }
        else {
            target.takeDamage(10.0f);   // Ĭ���˺�
        }
    }

    // ��Ҷ��صļ���
    void castSkill() {
        CCLOG("Player casts a special skill!");
    }

    // ��ӡ���״̬
    void printStatus() override {
        Spirit::printStatus();
        CCLOG("Experience: %d", experience);
        CCLOG("Level: %d", level);
    }
};

#endif // PLAYERSPIRIT_H

