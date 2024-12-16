#include "SpiritManager.h"
#include "cocos2d.h"
#include <limits.h>

// ���캯��
SpiritManager::SpiritManager() : nearestEnemy(nullptr) {}

void SpiritManager::init(BlockManager* bm, Player* theplayer) {
    blockManager = bm;
    player = theplayer;
}

// ������Ҿ����ָ��
void SpiritManager::setPlayer(Player* player) {
    this->player = player;
}

// ��ȡ��Ҿ����ָ��
Player* SpiritManager::getPlayer() const {
    return player;
}

// ��ȡ����Ĺ���
Enemy* SpiritManager::getNearestEnemy() const {
    return nearestEnemy;
}

// ��ȡ���˵���ҵľ���ӳ��
const std::unordered_map<Enemy*, float>& SpiritManager::getEnemyDistances() const {
    return enemyDistances;
}

// ������Һ������ڹ����״̬
void SpiritManager::update(float deltatime) {
    if (player == nullptr) {
        CCLOG("Player is not set!");
        return;
    }

    if (blockManager) {
        // ���������ڹ���״̬��������ж������
        blockManager->updateBlocksForPlayer(player);
    }
    // �����Ҫ�����������Χ�Ĺ��������״̬����AI�ȣ�
    updateDistances();
    updateEnemy(deltatime);
}

// ������������֮����������
void SpiritManager::updateDistances() {
    if (player == nullptr) {
        CCLOG("Player is not set!");
        return;
    }


    // ��ȡ��ҵ�λ��
    cocos2d::Vec2 playerPos = player->getPosition();

    // ��ȡ������ڵ�����
    std::pair<int, int> playerBlock = blockManager->getBlockCoordinates(playerPos);

    // ��ȡ������������ڵ����е���
    std::vector<Enemy*> enemiesInBlock = blockManager->getEnemiesInBlock(playerBlock);

    // �洢����Ĺ������̾���
    Enemy* nearest = nullptr;
    float minDistance = FLT_MAX;

    // ���µ��˵���ҵľ���ӳ��
    enemyDistances.clear(); // ��վɵľ�������
    for (auto it = enemiesInBlock.begin(); it != enemiesInBlock.end();) {
        Enemy* enemy = *it;
        if (!enemy->getIsAlive()) {
            // ɾ�����˲����µ�����
            it = enemiesInBlock.erase(it);
            continue; // ��������������������ѭ�������������һ������
        }

        // ������ҵ�����ľ���
        float distance = playerPos.distance(enemy->getPosition());

        // ���¾���ӳ��
        enemyDistances[enemy] = distance;

        // �ҵ�����Ĺ���
        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;
        }

        // ���ӵ���������һ��Ԫ��
        ++it;
    }

    // ��������Ĺ���
    if (nearest != nearestEnemy) {
        nearestEnemy = nearest;
    }
}

void SpiritManager::updateEnemy(float deltatime){
    if (nearestEnemy) {
        // �������еĵ��˺����ǵ���ҵľ���
        for (const auto& entry : enemyDistances) {
            Enemy* enemy = entry.first;
            float distance = entry.second;


            enemy->update(deltatime);
            // ���ù���� AI ��Ϊ����������ҵĵȼ��͵��˾���
            enemy->aiBehavior(distance, player);

        }
    }
}
