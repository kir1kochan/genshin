#include "BlockManager.h"
#include "cocos2d.h"

USING_NS_CC;

BlockManager::BlockManager(std::string& filePath) {
    // 加载 TMX 文件的对象层数据
    //loadObjectsFromTMX("filePath");
}

BlockManager::BlockManager() {
}

BlockManager::~BlockManager() {
    clear();
}

// 获取位置所在的区块
std::pair<int, int> BlockManager::getBlockCoordinates(const Vec2& position) const {
    int x = static_cast<int>(std::floor(position.x / BLOCK_SIZE));
    int y = static_cast<int>(std::floor(position.y / BLOCK_SIZE));
    return std::make_pair(x, y);
}

// 更新区块内容，加载玩家附近的区块，卸载远离的区块
void BlockManager::updateBlocksForPlayer(Player* playerNode) {
    if (!playerNode) return;

    std::pair<int, int> newPlayerBlock = getBlockCoordinates(playerNode->getPosition());

    // 判断玩家所在区块是否发生变化
    if (newPlayerBlock == playerBlock) {
        return;
    }
    playerBlock = newPlayerBlock;
    updateCollisionMap(); // 更新碰撞映射

    blockStatus.clear();  // 清空原先的状态

    // 加载玩家周围的区块
    for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
        for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
            std::pair<int, int> blockToLoad = { playerBlock.first + dx, playerBlock.second + dy };

            // 如果区块已经被加载过了，就跳过
            if (loadedBlocks.find(blockToLoad) != loadedBlocks.end()) {
                continue;  // 区块已经加载，跳过处理
            }

            blockStatus[blockToLoad] = true; // 标记为需要加载

            // 将区块加入已加载列表
            if (loadedBlocks.find(blockToLoad) == loadedBlocks.end()) {
                loadedBlocks.insert(blockToLoad); // 添加到已加载的区块集合中
            }
        }
    }

    // 计算要卸载的区块
    for (auto it = loadedBlocks.begin(); it != loadedBlocks.end();) {
        const auto& block = *it;
        if (blockStatus.find(block) == blockStatus.end()) {
            int dx = std::abs(block.first - playerBlock.first);
            int dy = std::abs(block.second - playerBlock.second);
            if (dx > UNLOAD_RADIUS || dy > UNLOAD_RADIUS) {
                blockStatus[block] = false; // 标记为需要卸载
                it = loadedBlocks.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}


// 读取当前区块的加载状态
std::unordered_map<std::pair<int, int>, bool, BlockManager::PairHash> BlockManager::getBlockStatus() const {
    return blockStatus;
}


// 从 TMX 文件加载对象层的数据
void BlockManager::loadObjectsFromTMX(const std::string& tmxFile) {
    // 加载 TMX 文件
    TMXTiledMap* tmxMap = TMXTiledMap::create(tmxFile);
    if (!tmxMap) {
        CCLOG("Failed to load TMX file: %s", tmxFile.c_str());
        return;
    }

    // 获取对象层（假设对象层的名字为 "ObjectLayer"）
    TMXObjectGroup* objectGroup = tmxMap->getObjectGroup("ObjectLayer");
    if (!objectGroup) {
        CCLOG("No object layer found in TMX file: %s", tmxFile.c_str());
        return;
    }

    // 获取对象层中的所有对象
    auto objects = objectGroup->getObjects();
    for (auto& obj : objects) {
        // 解析每个对象的类型和位置
        ValueMap objectData = obj.asValueMap();
        std::string type = objectData["type"].asString();
        float x = objectData["x"].asFloat();
        float y = objectData["y"].asFloat();
        // 将对象添加到相应的区块中
        std::pair<int, int> block = getBlockCoordinates(Vec2(x, y));
        if (type == "SpecialEnemy") {
            std::string jsonpath = objectData["jsonpath"].asString();
            // 创建敌人并加入到区块
            Enemy* enemy = new Enemy();
            enemy->loadFromFile(jsonpath);
            enemy->setPosition(Vec2(x, y));
            blockToEnemies[block].push_back(enemy);
        }
        else if (type == "Enemy") {
            // 读取敌人特有数据，如生命值、攻击力等
            float health = objectData["health"].asFloat();
            float attack = objectData["attack"].asFloat();
            float defence = objectData["defence"].asFloat();
            std::string element = objectData["element"].asString();  
            float attackRange = objectData["attackRange"].asFloat();
            int aggressionLevel = objectData["aggressionLevel"].asInt();
            float detectionRadius = objectData["detectionRadius"].asFloat();
            int baseLevel = objectData["baseLevel"].asInt();
            std::string imagePath = objectData["imagePath"].asString();  // 纹理路径
            int drop = objectData["drop"].asInt();  // 掉落物品ID

            // 创建敌人并加入到区块
            Enemy* enemy = new(std::nothrow) Enemy(
                health, attack, defence,
                static_cast<Element>(std::stoi(element)), attackRange,
                aggressionLevel, detectionRadius,
                baseLevel, imagePath, drop
            );
            enemy->setPosition(Vec2(x, y));
            blockToEnemies[block].push_back(enemy);
        }
        else if (type == "SceneObject") {
            SceneObject* sceneObject = new SceneObject();
            std::string subtype = objectData["subtype"].asString();
            if (subtype == "door") {    // 目前设想的特殊物体
                std::string jsonpath = objectData["jsonpath"].asString();
                SceneObject* sceneObject = new SceneObject();
                // sceneObject->loadFromFile(jsonpath);
            }
            else {
                // 读取场景物体特有数据
                std::string imagePath = objectData["imagePath"].asString();  // 物体纹理路径
                // 创建场景物体
                SceneObject* sceneObject = new(std::nothrow) SceneObject(stringToObjectType(subtype), Vec2(x, y), imagePath);
                // 解析并存储多个物品ID
                std::string itemIdsStr = objectData["itemIds"].asString();  // 存储为逗号分隔的字符串
                std::istringstream itemIdsStream(itemIdsStr);
                std::string itemId;
                while (std::getline(itemIdsStream, itemId, ',')) {
                    if (!itemId.empty()) {
                        sceneObject->addItemId(std::stoi(itemId));
                    }
                }      
                
                // 添加到区块
                blockToSceneObjects[block].push_back(sceneObject);
            }
            sceneObject->setPosition(Vec2(x, y));
            blockToSceneObjects[block].push_back(sceneObject);
        }
        else if (type == "Collision") { // 碰撞区域处理
            // 检查是否有 width 和 height 字段
            if (objectData.find("width") != objectData.end() && objectData.find("height") != objectData.end()) {
                float width = objectData["width"].asFloat();
                float height = objectData["height"].asFloat();

                // 创建一个矩形碰撞区域
                Rect collisionRect(x, y, width, height);

                // 保存到碰撞区域列表
                collisionAreas.push_back(collisionRect);
            }
        }
    }
}

// 处理点击事件（比如采摘、烹饪、钓鱼点等）
void BlockManager::handleClickEvent(const Vec2& clickPosition) {
    checkCollisions(clickPosition);
}

// 检查玩家点击位置与场景物体或敌人的碰撞
void BlockManager::checkCollisions(const Vec2& clickPosition) {
    std::pair<int, int> block = getBlockCoordinates(clickPosition);

    // 检查场景物体
    if (blockToSceneObjects.find(block) != blockToSceneObjects.end()) {
        for (auto& sceneObject : blockToSceneObjects[block]) {
            if (sceneObject->getBoundingBox().containsPoint(clickPosition)) {
                CCLOG("SceneObject clicked!");
                // 执行场景物体的点击事件处理
            }
        }
    }
}

std::vector<Enemy*> BlockManager::getEnemyInPlayerBlock() {
    return getEnemiesInBlock(playerBlock);
}

std::vector<Enemy*> BlockManager::getEnemiesInBlock(const std::pair<int, int>& block) {
    auto it = blockToEnemies.find(block);
    if (it != blockToEnemies.end()) {
        return it->second;
    }
    return {}; // 返回空向量;
}


// 清理区块
void BlockManager::clear() {
    for (auto& block : blockToEnemies) {
        for (auto& enemy : block.second) {
            delete enemy; 
        }
    }

    for (auto& block : blockToSceneObjects) {
        for (auto& sceneObject : block.second) {
            delete sceneObject;
        }
    }

    loadedBlocks.clear();
    blockToEnemies.clear();
    blockToSceneObjects.clear();
}


std::vector<SceneObject*> BlockManager::getSceneObjectsInBlock(const std::pair<int, int>& block) {
    auto it = blockToSceneObjects.find(block);
    if (it != blockToSceneObjects.end()) {
        return it->second;
    }
    return {};
}

void BlockManager::updateCollisionMap() {
    // 清除原有的映射
    sceneObjectCollisions.clear();

    // 遍历当前玩家所在区块及其周围区块
    for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
        for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
            std::pair<int, int> blockToUpdate = { playerBlock.first + dx, playerBlock.second + dy };

            // 获取该区块中的所有场景物体
            if (blockToSceneObjects.find(blockToUpdate) != blockToSceneObjects.end()) {
                for (auto* sceneObject : blockToSceneObjects[blockToUpdate]) {
                    // 将场景物体及其碰撞框添加到映射中
                    sceneObjectCollisions[blockToUpdate][sceneObject] = sceneObject->getBoundingBox();
                }
            }
        }
    }
}

void BlockManager::addEnemy(Enemy* enemy) {
    std::pair<int, int>block= getBlockCoordinates(enemy->getPosition());
    blockToEnemies[block].push_back(enemy);
}