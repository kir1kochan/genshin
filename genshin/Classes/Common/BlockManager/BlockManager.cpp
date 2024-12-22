#include "BlockManager.h"
#include "cocos2d.h"

USING_NS_CC;

BlockManager::BlockManager(std::string& filePath) {
    // ���� TMX �ļ��Ķ��������
    loadObjectsFromTMX(filePath);
}

BlockManager::BlockManager() {
}

BlockManager::~BlockManager() {
    clear();
}

// ��ȡλ�����ڵ�����
std::pair<int, int> BlockManager::getBlockCoordinates(const Vec2& position) const {
    int x = static_cast<int>(std::floor(position.x / BLOCK_SIZE));
    int y = static_cast<int>(std::floor(position.y / BLOCK_SIZE));
    return std::make_pair(x, y);
}

// �����������ݣ�������Ҹ��������飬ж��Զ�������
void BlockManager::updateBlocksForPlayer(Player* playerNode) {
    if (!playerNode) return;

    std::pair<int, int> newPlayerBlock = getBlockCoordinates(playerNode->getPosition());

    // �ж�������������Ƿ����仯
    if (newPlayerBlock == playerBlock) {
        return;
    }
    playerBlock = newPlayerBlock;

    blockStatus.clear();  // ���ԭ�ȵ�״̬

    // ���������Χ������
    for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
        for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
            std::pair<int, int> blockToLoad = { playerBlock.first + dx, playerBlock.second + dy };

            // ��������Ѿ������ع��ˣ�������
            if (loadedBlocks.find(blockToLoad) != loadedBlocks.end()) {
                continue;  // �����Ѿ����أ���������
            }

            blockStatus[blockToLoad] = true; // ���Ϊ��Ҫ����
            auto SOs = getSceneObjectsInBlock(blockToLoad);
            for (auto SO : SOs) {
                SO->generateSpriteIfNeeded();
            }
            auto enemys = getEnemiesInBlock(blockToLoad);
            for (auto enemy : enemys) {
                enemy->generateSprite();
            }
            // ����������Ѽ����б�
            if (loadedBlocks.find(blockToLoad) == loadedBlocks.end()) {
                loadedBlocks.insert(blockToLoad); // ��ӵ��Ѽ��ص����鼯����
            }
        }
    }
    updateCollisionMap(); // ������ײӳ��

    // ����Ҫж�ص�����
    for (auto it = loadedBlocks.begin(); it != loadedBlocks.end();) {
        const auto& block = *it;
        if (blockStatus.find(block) == blockStatus.end()) {
            int dx = std::abs(block.first - playerBlock.first);
            int dy = std::abs(block.second - playerBlock.second);
            if (dx > UNLOAD_RADIUS || dy > UNLOAD_RADIUS) {
                blockStatus[block] = false; // ���Ϊ��Ҫж��
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


// ��ȡ��ǰ����ļ���״̬
std::unordered_map<std::pair<int, int>, bool, BlockManager::PairHash> BlockManager::getBlockStatus() const {
    return blockStatus;
}


// �� TMX �ļ����ض���������
void BlockManager::loadObjectsFromTMX(const std::string& tmxFile) {
    // ���� TMX �ļ�
    TMXTiledMap* tmxMap = TMXTiledMap::create(tmxFile);
    if (!tmxMap) {
        CCLOG("Failed to load TMX file: %s", tmxFile.c_str());
        return;
    }
    auto sence = Director::getInstance()->getRunningScene();
    tmxMap->setName("background");
    sence->addChild(tmxMap, -1);  // ����ͼ��Ϊ��������ӵ�������

    // ����ײ����ͼ���ע�����ò��ɼ�
    auto area = tmxMap->getLayer("area");
    area->setVisible(false);

    // ��ȡ����㣨�������������Ϊ "ObjectLayer"��
    TMXObjectGroup* objectGroup = tmxMap->getObjectGroup("Objects");
    if (!objectGroup) {
        CCLOG("No object layer found in TMX file: %s", tmxFile.c_str());
        return;
    }

    // ��ȡ������е����ж���
    auto objects = objectGroup->getObjects();
    for (auto& obj : objects) {
        // ����ÿ����������ͺ�λ��
        ValueMap objectData = obj.asValueMap();
        std::string type = objectData["name"].asString();
        float x = objectData["x"].asFloat();
        float y = objectData["y"].asFloat();
        // ��������ӵ���Ӧ��������
        std::pair<int, int> block = getBlockCoordinates(Vec2(x, y));
        if (type == "SpecialEnemy") {
            std::string jsonpath = objectData["jsonpath"].asString();
            // �������˲����뵽����
            Enemy* enemy = new Enemy();
            enemy->loadFromFile(jsonpath);
            enemy->setPosition(Vec2(x, y));
            blockToEnemies[block].push_back(enemy);
        }
        else if (type == "Enem") {
            // ��ȡ�����������ݣ�������ֵ����������
            float health = objectData["health"].asFloat();
            float attack = objectData["attack"].asFloat();
            float defence = objectData["defence"].asFloat();
            std::string element = objectData["element"].asString();
            float attackRange = objectData["attackRange"].asFloat();
            int aggressionLevel = objectData["aggressionLevel"].asInt();
            float detectionRadius = objectData["detectionRadius"].asFloat();
            int baseLevel = objectData["baseLevel"].asInt();
            std::string imagePath = objectData["imagePath"].asString();  // ����·��
            int drop = objectData["drop"].asInt();  // ������ƷID

            // �������˲����뵽����
            Enemy* enemy = new(std::nothrow) Enemy(
                health, attack, defence,
                stringToElement(element), attackRange,
                aggressionLevel, detectionRadius,
                baseLevel, imagePath, drop
            );
            enemy->setPosition(Vec2(x, y));
            blockToEnemies[block].push_back(enemy);
        }
        else if (type == "Collection" || type == "Fish") {
            SceneObject* sceneObject = new SceneObject();
            std::string subtype = objectData["subtype"].asString();
            if (subtype == "door") {    // Ŀǰ�������������
                std::string jsonpath = objectData["jsonpath"].asString();
                SceneObject* sceneObject = new SceneObject();
                // sceneObject->loadFromFile(jsonpath);
                sceneObject->setPosition(Vec2(x, y));
                blockToSceneObjects[block].push_back(sceneObject);

                // ���� door ����
                int index = objectData["index"].asInt();
                float width = objectData["width"].asFloat();
                float height = objectData["height"].asFloat();
                Rect doorRect(x, y, width, height);
                doorAreas[doorRect] = index;
            }
            else {
                // ��ȡ����������������
                std::string imagePath = objectData["imagePath"].asString();  // ��������·��
                // ������������
                SceneObject* sceneObject = new(std::nothrow) SceneObject(stringToObjectType(type), Vec2(x, y), imagePath);
                // �������洢�����ƷID
                std::string itemIdsStr = objectData["itemIds"].asString();  // �洢Ϊ���ŷָ����ַ���
                std::istringstream itemIdsStream(itemIdsStr);
                std::string itemId;
                while (std::getline(itemIdsStream, itemId, ',')) {
                    if (!itemId.empty()) {
                        sceneObject->addItemId(std::stoi(itemId));
                    }
                }

                // ��ӵ�����
                blockToSceneObjects[block].push_back(sceneObject);
            }
        }
        else if (type == "Collision") { // ��ײ������
            // ����Ƿ��� width �� height �ֶ�
            if (objectData.find("width") != objectData.end() && objectData.find("height") != objectData.end()) {
                float width = objectData["width"].asFloat();
                float height = objectData["height"].asFloat();

                // ����һ��������ײ����
                Rect collisionRect(x, y, width, height);

                // ���浽��ײ�����б�
                collisionAreas.push_back(collisionRect);
            }
        }
    }
}

// �������¼��������ժ����⿡������ȣ�
void BlockManager::handleClickEvent(const Vec2& clickPosition, Player* player) {
    checkCollisions(clickPosition, player);
}

// �����ҵ��λ���볡���������˵���ײ
void BlockManager::checkCollisions(const Vec2& clickPosition, Player* player) {
    std::pair<int, int> block = getBlockCoordinates(clickPosition);

    // ��鳡������
    if (blockToSceneObjects.find(block) != blockToSceneObjects.end()) {
        for (auto& sceneObject : blockToSceneObjects[block]) {
            auto rect = Rect(sceneObject->getPosition() - Vec2(36, 36), Size(72, 72));
            if (rect.containsPoint(clickPosition)) {
                CCLOG("SceneObject clicked!");
                sceneObject->interactWithPlayer(player->getBackpack());
                // ִ�г�������ĵ���¼�����
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
    return {}; // ���ؿ�����;
}


// ��������
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
    // ���ԭ�е�ӳ��
    sceneObjectCollisions.clear();

    // ������ǰ����������鼰����Χ����
    for (int dx = -LOAD_RADIUS; dx <= LOAD_RADIUS; ++dx) {
        for (int dy = -LOAD_RADIUS; dy <= LOAD_RADIUS; ++dy) {
            std::pair<int, int> blockToUpdate = { playerBlock.first + dx, playerBlock.second + dy };

            // ��ȡ�������е����г�������
            if (blockToSceneObjects.find(blockToUpdate) != blockToSceneObjects.end()) {
                for (auto* sceneObject : blockToSceneObjects[blockToUpdate]) {
                    // ���������弰����ײ����ӵ�ӳ����
                    sceneObjectCollisions[blockToUpdate][sceneObject] = sceneObject->getBoundingBox();
                }
            }
        }
    }
}

void BlockManager::addEnemy(Enemy* enemy) {
    std::pair<int, int>block = getBlockCoordinates(enemy->getPosition());
    blockToEnemies[block].push_back(enemy);
}

int BlockManager::checkPlayerInDoorArea(const cocos2d::Vec2& playerPosition) {
    for (const auto& doorArea : doorAreas) {
        if (doorArea.first.containsPoint(playerPosition)) {
            return doorArea.second;
        }
    }
    return -1; // ���� -1 ��ʾ��Ҳ����κ� door ����
}

int BlockManager::checkPlayerInSmallMapDoorArea(const cocos2d::Vec2& playerPosition) {
    for (const auto& doorArea : doorAreas) {
        if (doorArea.first.containsPoint(playerPosition)) {
            return doorArea.second;
        }
    }
    return -1; // ���� -1 ��ʾ��Ҳ����κ� door ����
}