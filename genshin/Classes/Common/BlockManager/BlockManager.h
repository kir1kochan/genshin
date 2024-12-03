#ifndef __BLOCK_MANAGER_H__
#define __BLOCK_MANAGER_H__

#include "cocos2d.h"
#include "NonPlayerGenerator.h"
#include "Enemy.h"
#include "SceneObject.h"
#include <unordered_map>
#include <vector>
#include <set>

// �����С����
#define BLOCK_SIZE 128
#define LOAD_RADIUS 3
#define UNLOAD_RADIUS 4

namespace std {
    template <typename T1, typename T2>
    struct hash<pair<T1, T2>> {
        size_t operator()(const pair<T1, T2>& p) const {
            return hash<T1>()(p.first) ^ hash<T2>()(p.second);
        }
    };
}

class BlockManager {
public:
    BlockManager();
    ~BlockManager();

    // ��ӹ��ﵽ�ض�����
    void addEnemyToBlock(Enemy* enemy);

    // ��ȡ������ڵ�����
    std::pair<int, int> getBlockCoordinates(const cocos2d::Vec2& position) const;

    // ��ȡĳ�������ڵĹ���
    std::vector<Enemy*> getEnemiesInBlock(const std::pair<int, int>& blockCoordinates);

    // ��ȡĳ�������ڵĳ�������
    std::vector<SceneObject*> getSceneObjectsInBlock(const std::pair<int, int>& blockCoordinates);

    // �����������ݣ�������Ҹ��������飬ж��Զ�������
    void updateBlocksForPlayer(cocos2d::Node* playerNode);

    // ��������
    void clear();

private:
    // �������ɷ���ҵĳ�������͵���
    NonPlayerGenerator* nonPlayerGenerator;

    // �洢ÿ�������еĵ���
    std::unordered_map<std::pair<int, int>, std::vector<Enemy*>, std::hash<int>> blockToEnemies;

    // �洢ÿ�������еĳ�������
    std::unordered_map<std::pair<int, int>, std::vector<SceneObject*>, std::hash<int>> blockToSceneObjects;

    // �洢�Ѽ��ص�����
    std::set<std::pair<int, int>> loadedBlocks;

    // �� NonPlayerGenerator �м������е��˺ͳ�������λ�ò��洢��������
    void loadNonPlayerPositions();

    // δ���ĵ�ͼ����Ҳ������������
};

#endif // __BLOCK_MANAGER_H__
