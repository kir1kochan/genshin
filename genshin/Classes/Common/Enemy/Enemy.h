#ifndef ENEMY_H
#define ENEMY_H

#include "../Creator.h"

class Enemy : public Creator {
private:
    int aggressionLevel;  // ���˵Ĺ����Եȼ����������˵Ĺ���Ƶ�ʻ�ǿ�ȣ�

public:
    Enemy(float health, Element element, int aggressionLevel);
    Enemy();  // Ĭ�Ϲ��캯��

    int getAggressionLevel() const;

    // ��������
    void attack(Creator& target) override;

    // ����AI��Ϊ
    void aiBehavior();

    // ��ӡ����״̬
    void printStatus() override;
};

#endif // ENEMY_H
