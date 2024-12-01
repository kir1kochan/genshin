#include "SpiritManager.h"
#include "Enemy.h"
#include <unordered_map>
#include <limits.h>

// ���캯��
SpiritManager::SpiritManager() : nearestEnemy(nullptr) {
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

    // Ѱ������Ĺ���
    Enemy* nearest = nullptr;
    float minDistance = FLT_MAX;

    for (Enemy* enemy : enemiesInBlock) {
        // ������ҵ�����ľ���
        float distance = playerPos.distance(enemy->getPosition());

        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;
        }
    }

    // ��������Ĺ���
    if (nearest != nearestEnemy) {
        nearestEnemy = nearest;
        if (nearestEnemy) {
            CCLOG("Nearest enemy ID: %d, Distance: %.2f", nearestEnemy->getMonsterID(), minDistance);
        }
    }
}
