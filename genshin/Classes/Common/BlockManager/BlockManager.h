#ifndef __BLOCK_MANAGER_H__
#define __BLOCK_MANAGER_H__

#include "cocos2d.h"
#include "../../Scene/SceneObject/SceneObject.h"
#include "../Entities/Enemy/Enemy.h"
#include "../Entities/Player/Player.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <utility>

#define BLOCK_SIZE 1280
#define LOAD_RADIUS 2
#define UNLOAD_RADIUS 3

class BlockManager {
public:
    struct PairHash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };
    BlockManager();
    BlockManager(std::string& filePath);
    ~BlockManager();

    std::pair<int, int> getBlockCoordinates(const cocos2d::Vec2& position) const;
    void updateBlocksForPlayer(Player* playerNode);
    void handleClickEvent(const cocos2d::Vec2& clickPosition, Player* player);
    void clear();
    
    // �����������������������ײ���ӳ��
    void updateCollisionMap();

    // ��ӹ���
    void addEnemy(Enemy* enemy);

    // ��ȡָ����������е���
    std::vector<Enemy*> getEnemiesInBlock(const std::pair<int, int>& block);
    // ��ȡ�����������ĵ���
    std::vector<Enemy*> getEnemyInPlayerBlock();

    // ��ȡָ����������г�������
    std::vector<SceneObject*> getSceneObjectsInBlock(const std::pair<int, int>& block);
    std::unordered_map<std::pair<int, int>, bool, BlockManager::PairHash> getBlockStatus() const;

private:
    

    // �� TMX �ļ����ض�������
    void loadObjectsFromTMX(const std::string& tmxFile);

    // �������¼�
    void checkCollisions(const cocos2d::Vec2& clickPosition, Player* player);

    // �洢����״̬��trueΪ���أ�falseΪж�أ�
    std::unordered_map<std::pair<int, int>, bool, PairHash> blockStatus;

    // �Ѽ������鼯��
    std::set<std::pair<int, int>> loadedBlocks;

    // �洢�����ڵĵ���
    std::unordered_map<std::pair<int, int>, std::vector<Enemy*>, PairHash> blockToEnemies;

    // �洢�����ڵĳ�������
    std::unordered_map<std::pair<int, int>, std::vector<SceneObject*>, PairHash> blockToSceneObjects;

    // ��ҵ�ǰ���ڵ�����
    std::pair<int, int> playerBlock = { 0, 0 };
    // �洢��ҵ�ǰ���������SceneObject����ײ���ʵ����ӳ��
    std::unordered_map<std::pair<int, int>, std::unordered_map<SceneObject*, cocos2d::Rect>, PairHash> sceneObjectCollisions;
    // �洢��ײ����
    std::vector<cocos2d::Rect> collisionAreas;
};


#endif // __BLOCK_MANAGER_H__
