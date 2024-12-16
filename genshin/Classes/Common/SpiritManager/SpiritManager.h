#ifndef __SPIRIT_MANAGER_H__
#define __SPIRIT_MANAGER_H__

#include "cocos2d.h"
#include "../Entities/Enemy/Enemy.h"
#include "../Entities/Player/Player.h"
#include "../BlockManager/BlockManager.h"
#include <unordered_map>

class SpiritManager {
private:
    // ��Ҿ����ָ��
    Player* player = nullptr;

    // ��������������ڹ�������͹���
    BlockManager* blockManager = nullptr;

    // �洢����Ĺ���
    Enemy* nearestEnemy = nullptr;

    // �洢������ÿ�����������֮��ľ���
    std::unordered_map<Enemy*, float> enemyDistances;

public:
    // ���캯��
    SpiritManager();

    void init(BlockManager* bm, Player* theplayer);

    // ������Ҿ����ָ��
    void setPlayer(Player* player);

    // ��ȡ��Ҿ����ָ��
    Player* getPlayer() const;

    // ��ȡ����Ĺ���
    Enemy* getNearestEnemy() const;

    // ��ȡ���˵���ҵľ���ӳ��
    const std::unordered_map<Enemy*, float>& getEnemyDistances() const;

    // ������Һ������ڹ����״̬
    void update(float deltatime);

    // ������������֮����������
    void updateDistances();

    // ���¹�����Ϊ
    void updateEnemy(float deltatime);
};

#endif // __SPIRIT_MANAGER_H__
