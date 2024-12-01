#ifndef __SPIRIT_MANAGER_H__
#define __SPIRIT_MANAGER_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Player.h"
#include "BlockManager.h"

class SpiritManager {
private:
    // ��Ҿ����ָ��
    Player* player = nullptr;

    // ��������������ڹ�������͹���
    BlockManager blockManager;

    // �洢����Ĺ���
    Enemy* nearestEnemy = nullptr;

public:
    // ���캯��
    SpiritManager();

    // ������Ҿ����ָ��
    void setPlayer(Player* player);

    // ��ȡ��Ҿ����ָ��
    Player* getPlayer() const;

    // ��ȡ����Ĺ���
    Enemy* getNearestEnemy() const;

    // ������Һ������ڹ����״̬
    void update();

    // ������������֮����������
    void updateDistances();
};

#endif // __SPIRIT_MANAGER_H__
