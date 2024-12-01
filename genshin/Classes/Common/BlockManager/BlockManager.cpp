#include "BlockManager.h"

BlockManager::BlockManager()
    : nonPlayerGenerator(new NonPlayerGenerator()) {
    // 加载非玩家数据
    nonPlayerGenerator->loadNonPlayerDataFromJson("path_to_your_json_file.json");

    // 从 NonPlayerGenerator 中加载所有位置数据并存储到区块中
    loadNonPlayerPositions();
}

BlockManager::~BlockManager() {
    clear();
}

// 添加怪物到特定区块
void BlockManager::addEnemyToBlock(Enemy* enemy) {
    cocos2d::Vec2 enemyPosition = enemy->getPosition();
    std::pair<int, int> blockCoordinates = getBlockCoordinates(enemyPosition);
    blockToEnemies[blockCoordinates].push_back(enemy);
}

// 获取玩家所在的区块
std::pair<int, int> BlockManager::getBlockCoordinates(const cocos2d::Vec2& position) const {
    int x = static_cast<int>(std::floor(position.x / BLOCK_SIZE));
    int y = static_cast<int>(std::floor(position.y / BLOCK_SIZE));
    return std::make_pair(x, y);
}

// 获取某个区块内的怪物
std::vector<Enemy*> BlockManager::getEnemiesInBlock(const std::pair<int, int>& blockCoordinates) {
    if (blockToEnemies.find(blockCoordinates) != blockToEnemies.end()) {
        return blockToEnemies[blockCoordinates];
    }
    return {};
}

// 获取某个区块内的场景物体
std::vector<SceneObject*> BlockManager::getSceneObjectsInBlock(const std::pair<int, int>& blockCoordinates) {
    if (blockToSceneObjects.find(blockCoordinates) != blockToSceneObjects.end()) {
        return blockToSceneObjects[blockCoordinates];
    }
    return {};
}

// 更新区块内容，加载玩家附近的区块，卸载远离的区块
void BlockManager::updateBlocksForPlayer(cocos2d::Node* playerNode) {
    if (!playerNode) return;

    std::pair<int, int> playerBlock = getBlockCoordinates(playerNode->getPosition());

    std::set<std::pair<int, int>> blocksToLoad;
    // 加载玩家周围的区块（以玩家为中心，最大加载半径为 LOAD_RADIUS）
    for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
        for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
            std::pair<int, int> blockToLoad = { playerBlock.first + dx, playerBlock.second + dy };
            blocksToLoad.insert(blockToLoad);
        }
    }

    // 加载新的区块
    for (const auto& block : blocksToLoad) {
        if (loadedBlocks.find(block) == loadedBlocks.end()) {
            loadedBlocks.insert(block);
            CCLOG("Loading block: (%d, %d)", block.first, block.second);

            // 加载区块内的场景物体和敌人
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

    // 卸载远离的区块
    for (auto it = loadedBlocks.begin(); it != loadedBlocks.end();) {
        const auto& block = *it;
        if (blocksToLoad.find(block) == blocksToLoad.end()) {
            int dx = std::abs(block.first - playerBlock.first);
            int dy = std::abs(block.second - playerBlock.second);
            if (dx > UNLOAD_RADIUS || dy > UNLOAD_RADIUS) {
                CCLOG("Unloading block: (%d, %d)", block.first, block.second);
                // 卸载区块中的所有敌人
                for (auto& enemy : blockToEnemies[block]) {
                    delete enemy;
                }
                blockToEnemies[block].clear();

                // 卸载区块中的所有场景物体
                for (auto& sceneObject : blockToSceneObjects[block]) {
                    delete sceneObject;
                }
                blockToSceneObjects[block].clear();

                it = loadedBlocks.erase(it);  // 删除并继续遍历
            }
            else {
                ++it;  // 没有卸载则继续遍历
            }
        }
        else {
            ++it;  // 如果区块仍需要加载，继续遍历
        }
    }
}

// 清理区块
void BlockManager::clear() {
    // 清理敌人
    for (auto& block : blockToEnemies) {
        for (auto& enemy : block.second) {
            delete enemy;
        }
    }

    // 清理场景物体
    for (auto& block : blockToSceneObjects) {
        for (auto& sceneObject : block.second) {
            delete sceneObject;
        }
    }

    loadedBlocks.clear();
    blockToEnemies.clear();
    blockToSceneObjects.clear();
}

// 从 NonPlayerGenerator 中加载所有敌人和场景物体位置并存储到区块中
void BlockManager::loadNonPlayerPositions() {
    // 获取所有敌人位置
    auto enemyPositions = nonPlayerGenerator->getAllEnemyPositions();
    for (const auto& pair : enemyPositions) {
        cocos2d::Vec2 position = pair.first;
        std::pair<int, int> block = getBlockCoordinates(position);
        blockToEnemies[block].push_back(position);
    }

    // 获取所有场景物体位置
    auto sceneObjectPositions = nonPlayerGenerator->getAllSceneObjectPositions();
    for (const auto& pair : sceneObjectPositions) {
        cocos2d::Vec2 position = pair.first;
        std::pair<int, int> block = getBlockCoordinates(position);
        blockToSceneObjects[block].push_back(position);
    }
}
