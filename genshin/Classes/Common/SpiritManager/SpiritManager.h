#ifndef __SPIRIT_MANAGER_H__
#define __SPIRIT_MANAGER_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Player.h"
#include "BlockManager.h"
#include <unordered_map>

class SpiritManager {
private:
    // ��Ҿ����ָ��
    Player* player = nullptr;

    // ��������������ڹ�������͹���
    BlockManager blockManager;

    // �洢����Ĺ���
    Enemy* nearestEnemy = nullptr;

    // �洢������ÿ�����������֮��ľ���
    std::unordered_map<Enemy*, float> enemyDistances;

public:
    // ���캯��
    SpiritManager();

    // ������Ҿ����ָ��
    void setPlayer(Player* player);

    // ��ȡ��Ҿ����ָ��
    Player* getPlayer() const;

    // ��ȡ����Ĺ���
    Enemy* getNearestEnemy() const;

    // ��ȡ���˵���ҵľ���ӳ��
    const std::unordered_map<Enemy*, float>& getEnemyDistances() const;

    // ������Һ������ڹ����״̬
    void update();

    // ������������֮����������
    void updateDistances();

    // ���¹�����Ϊ
    void updateEnemy()
};

#endif // __SPIRIT_MANAGER_H__
