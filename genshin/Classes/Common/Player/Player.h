#ifndef PLAYER_H
#define PLAYER_H

#include "../Creator.h"

class Player : public Creator {
private:
    int experience;  // ��Ҿ���ֵ
    int level;       // ��ҵȼ�

public:
    Player(float health, Element element);
    Player();  // Ĭ�Ϲ��캯��

    // ����
    void levelUp();

    // ��ȡ��ǰ�ȼ�
    int getLevel() const;

    // ��������
    void gainExperience(int exp);

    // ������ҵ���ʱ����Ԫ�����
    void attack(Creator& target) override;

    // ��Ҷ��صļ���
    void castSkill();

    // ��ӡ���״̬
    void printStatus() override;
};

#endif // PLAYER_H
