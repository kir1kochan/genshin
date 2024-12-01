#include "BlockManager.h"

BlockManager::BlockManager()
    : nonPlayerGenerator(new NonPlayerGenerator()) {
    // ���ط��������
    nonPlayerGenerator->loadNonPlayerDataFromJson("path_to_your_json_file.json");

    // �� NonPlayerGenerator �м�������λ�����ݲ��洢��������
    loadNonPlayerPositions();
}

BlockManager::~BlockManager() {
    clear();
}

// ��ӹ��ﵽ�ض�����
void BlockManager::addEnemyToBlock(Enemy* enemy) {
    cocos2d::Vec2 enemyPosition = enemy->getPosition();
    std::pair<int, int> blockCoordinates = getBlockCoordinates(enemyPosition);
    blockToEnemies[blockCoordinates].push_back(enemy);
}

// ��ȡ������ڵ�����
std::pair<int, int> BlockManager::getBlockCoordinates(const cocos2d::Vec2& position) const {
    int x = static_cast<int>(std::floor(position.x / BLOCK_SIZE));
    int y = static_cast<int>(std::floor(position.y / BLOCK_SIZE));
    return std::make_pair(x, y);
}

// ��ȡĳ�������ڵĹ���
std::vector<Enemy*> BlockManager::getEnemiesInBlock(const std::pair<int, int>& blockCoordinates) {
    if (blockToEnemies.find(blockCoordinates) != blockToEnemies.end()) {
        return blockToEnemies[blockCoordinates];
    }
    return {};
}

// ��ȡĳ�������ڵĳ�������
std::vector<SceneObject*> BlockManager::getSceneObjectsInBlock(const std::pair<int, int>& blockCoordinates) {
    if (blockToSceneObjects.find(blockCoordinates) != blockToSceneObjects.end()) {
        return blockToSceneObjects[blockCoordinates];
    }
    return {};
}

// �����������ݣ�������Ҹ��������飬ж��Զ�������
void BlockManager::updateBlocksForPlayer(cocos2d::Node* playerNode) {
    if (!playerNode) return;

    std::pair<int, int> playerBlock = getBlockCoordinates(playerNode->getPosition());

    std::set<std::pair<int, int>> blocksToLoad;
    // ���������Χ�����飨�����Ϊ���ģ������ذ뾶Ϊ LOAD_RADIUS��
    for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
        for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
            std::pair<int, int> blockToLoad = { playerBlock.first + dx, playerBlock.second + dy };
            blocksToLoad.insert(blockToLoad);
        }
    }

    // �����µ�����
    for (const auto& block : blocksToLoad) {
        if (loadedBlocks.find(block) == loadedBlocks.end()) {
            loadedBlocks.insert(block);
            CCLOG("Loading block: (%d, %d)", block.first, block.second);

            // ���������ڵĳ�������͵���
            if (blockToSceneObjects.find(block) != blockToSceneObjects.end()) {
                for (const auto& position : blockToSceneObjects[block]) {
                    nonPlayerGenerator->generateSceneObjectFromJson(position);
                }
            }
            if (blockToEnemies.find(block) != blockToEnemies.end()) {
                for (const auto& position : blockToEnemies[block]) {
                    nonPlayerGenerator->generateEnemyFromJson(position);
                }
            }
        }
    }

    // ж��Զ�������
    for (auto it = loadedBlocks.begin(); it != loadedBlocks.end();) {
        const auto& block = *it;
        if (blocksToLoad.find(block) == blocksToLoad.end()) {
            int dx = std::abs(block.first - playerBlock.first);
            int dy = std::abs(block.second - playerBlock.second);
            if (dx > UNLOAD_RADIUS || dy > UNLOAD_RADIUS) {
                CCLOG("Unloading block: (%d, %d)", block.first, block.second);
                // ж�������е����е���
                for (auto& enemy : blockToEnemies[block]) {
                    delete enemy;
                }
                blockToEnemies[block].clear();

                // ж�������е����г�������
                for (auto& sceneObject : blockToSceneObjects[block]) {
                    delete sceneObject;
                }
                blockToSceneObjects[block].clear();

                it = loadedBlocks.erase(it);  // ɾ������������
            }
            else {
                ++it;  // û��ж�����������
            }
        }
        else {
            ++it;  // �����������Ҫ���أ���������
        }
    }
}

// ��������
void BlockManager::clear() {
    // �������
    for (auto& block : blockToEnemies) {
        for (auto& enemy : block.second) {
            delete enemy;
        }
    }

    // ����������
    for (auto& block : blockToSceneObjects) {
        for (auto& sceneObject : block.second) {
            delete sceneObject;
        }
    }

    loadedBlocks.clear();
    blockToEnemies.clear();
    blockToSceneObjects.clear();
}

// �� NonPlayerGenerator �м������е��˺ͳ�������λ�ò��洢��������
void BlockManager::loadNonPlayerPositions() {
    // ��ȡ���е���λ��
    auto enemyPositions = nonPlayerGenerator->getAllEnemyPositions();
    for (const auto& pair : enemyPositions) {
        cocos2d::Vec2 position = pair.first;
        std::pair<int, int> block = getBlockCoordinates(position);
        blockToEnemies[block].push_back(position);
    }

    // ��ȡ���г�������λ��
    auto sceneObjectPositions = nonPlayerGenerator->getAllSceneObjectPositions();
    for (const auto& pair : sceneObjectPositions) {
        cocos2d::Vec2 position = pair.first;
        std::pair<int, int> block = getBlockCoordinates(position);
        blockToSceneObjects[block].push_back(position);
    }
}
