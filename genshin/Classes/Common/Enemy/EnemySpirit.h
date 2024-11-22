#ifndef ENEMYSPIRIT_H
#define ENEMYSPIRIT_H

#include "../Spirit.h"

class EnemySpirit : public Spirit {
private:
    int aggressionLevel;  // ���˵Ĺ����Եȼ����������˵Ĺ���Ƶ�ʻ�ǿ�ȣ�

public:
    EnemySpirit(float health, Element element, int aggressionLevel)
        : Spirit(health, element), aggressionLevel(aggressionLevel) {}

    // ���ڲ��Ե�Ĭ�Ϲ��캯��
    EnemySpirit() : Spirit(100, Element::EARTH) {}

    int getAttack() const { return aggressionLevel; }

    // ��������
    void attack(Spirit& target) override {
        if (aggressionLevel > 5) {
            target.takeDamage(15.0f);  // �߹�����
        }
        else {
            target.takeDamage(5.0f);   // �͹�����
        }
    }

    // ����AI��Ϊ
    void aiBehavior() {
        if (health < maxHealth / 2) {
            CCLOG("Enemy retreats!");
        }
        else {
            CCLOG("Enemy attacks with high aggression!");
        }
    }

    // ��ӡ����״̬
    void printStatus() override {
        Spirit::printStatus();
        CCLOG("Aggression Level: %d", aggressionLevel);
    }
};

#endif // ENEMYSPIRIT_H

