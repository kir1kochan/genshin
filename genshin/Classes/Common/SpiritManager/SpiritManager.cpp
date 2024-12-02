#include "SpiritManager.h"
#include "cocos2d.h"
#include <limits.h>

// ���캯��
SpiritManager::SpiritManager() : nearestEnemy(nullptr) {}

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
void SpiritManager::update() {
    if (player == nullptr) {
        CCLOG("Player is not set!");
        return;
    }

    // ���������ڹ���״̬��������ж������
    blockManager.updateBlocksForPlayer(player);

    // �����Ҫ�����������Χ�Ĺ��������״̬����AI�ȣ�
    updateDistances();
    updateEnemies();
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
    std::pair<int, int> playerBlock = blockManager.getBlockCoordinates(playerPos);

    // ��ȡ������������ڵ����е���
    std::vector<Enemy*> enemiesInBlock = blockManager.getEnemiesInBlock(playerBlock);

    // �洢����Ĺ������̾���
    Enemy* nearest = nullptr;
    float minDistance = FLT_MAX;

    // ���µ��˵���ҵľ���ӳ��
    enemyDistances.clear(); // ��վɵľ�������
    for (Enemy* enemy : enemiesInBlock) {
        // ������ҵ�����ľ���
        float distance = playerPos.distance(enemy->getPosition());

        // ���¾���ӳ��
        enemyDistances[enemy] = distance;

        // �ҵ�����Ĺ���
        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;
        }
    }

    // ��������Ĺ���
    if (nearest != nearestEnemy) {
        nearestEnemy = nearest;
    }
}

void SpiritManager::updateEnemy(){
    if (nearestEnemy) {
        // �������еĵ��˺����ǵ���ҵľ���
        for (const auto& entry : enemyDistances) {
            Enemy* enemy = entry.first;
            float distance = entry.second;

            // ��ȡ��ҵĵȼ�
            int playerLevel = player->getLevel();

            // ���ù���� AI ��Ϊ����������ҵĵȼ��͵��˾���
            enemy->aiBehavior(distance, player);

        }
    }
}
