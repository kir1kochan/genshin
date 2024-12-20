#include "KeyboardEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void KeyboardEventManager::initialize() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyboardEventManager::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(KeyboardEventManager::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, Director::getInstance()->getRunningScene());
    defaultEnemy = new Enemy();
    defaultEnemy->setPosition(-100, -100);
    moveDirection.y = 0;
    moveDirection.x = 0;
}

void KeyboardEventManager::handleEvent() {
    // 处理键盘输入事件
}

// 键盘按下事件处理
void KeyboardEventManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (player == nullptr || isBackpackActive) return;  // 如果背包界面激活，忽略事件

    // 在按下某个按键时，记录该按键的方向
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        moveDirection.y = 1;  // 向上
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        moveDirection.y = -1;  // 向下
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        moveDirection.x = -1;  // 向左
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        moveDirection.x = 1;   // 向右
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
        if (nearestEnemy) {
            player->useSkill(0, *nearestEnemy);
        }
        player->useSkill(0, *defaultEnemy);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_R) {
        if (nearestEnemy) {
            player->useSkill(1, *nearestEnemy);
        }
        player->useSkill(1, *defaultEnemy);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_E && !isBackpackActive) {
        if (nearestEnemy) {
            player->useSkill(2, *nearestEnemy);
        }
        player->useSkill(2, *defaultEnemy);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_1 && !isBackpackActive) {
        if (nearestEnemy) {
            player->useSkill(3, *nearestEnemy);
        }
        player->useSkill(3, *defaultEnemy);
        
    }
}

// 键盘按键释放事件处理
void KeyboardEventManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        moveDirection.y = 0;  // 停止向上
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        moveDirection.y = 0;  // 停止向下
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        moveDirection.x = 0;  // 停止向左
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        moveDirection.x = 0;  // 停止向右
    }
}

// 每帧更新玩家位置
void KeyboardEventManager::update(float deltaTime,CCTMXTiledMap* map) {
    if (player == nullptr || isBackpackActive) return;  // 如果背包界面激活，忽略更新

    // 计算目标位置
    Vec3 currentPos = player->getPosition3D();
    Vec3 targetPos = currentPos;
    targetPos.x += moveDirection.x * moveSpeed * deltaTime;
    targetPos.y += moveDirection.y * moveSpeed * deltaTime;

    //获取地图大小和图块大小
    auto mapSize = map->getMapSize();
    auto tileSize = map->getTileSize();
    //限定终点不能超范围
    //targetPos.y = targetPos.y > 0 ? (targetPos.y < mapSize.height * tileSize.height ? targetPos.y : mapSize.height * tileSize.height) : 0;
   // targetPos.x = targetPos.x > 0 ? (targetPos.x < mapSize.width * tileSize.width ? targetPos.x : mapSize.width * tileSize.width) : 0;

    //获取碰撞区域图层
    auto layer = map->getLayer("area");
    //获取当前图块坐标图块GID
    int tileGid = layer->getTileGIDAt(Vec2(targetPos.x / tileSize.width, (mapSize.height * tileSize.height - targetPos.y) / tileSize.height));
    destination = targetPos;

    if (tileGid) {
        // 获取 GID 对应的属性
        auto properties = map->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty()) {
            // 检查属性中是否有 "Collide"且为"true"
            auto collision = properties["collide"].asString();
            if (!collision.empty() && collision == "true") {
                destination = currentPos;
            }
        }
    }

    // 如果目标位置发生变化，创建动作
    if (destination != currentPos) {

        // 停止之前的动作，创建新的平滑移动动作
        player->stopAllActions(); // 避免残留动作
        auto moveAction = MoveTo::create(deltaTime, Vec2(destination.x, destination.y)); // 0.2 秒内移动到目标
        player->runAction(moveAction);

        // 更新相机动作
        if (auto camera = Director::getInstance()->getRunningScene()->getDefaultCamera()) {
            Vec3 cameraTargetPos = Vec3(destination.x, destination.y, camera->getPosition3D().z);
            camera->stopAllActions(); // 停止之前的相机动作
            auto cameraMoveAction = MoveTo::create(deltaTime, Vec3(cameraTargetPos.x, cameraTargetPos.y,  camera->getPosition3D().z)); // 稍微延迟
            camera->runAction(cameraMoveAction);
        }
    }
}

void KeyboardEventManager::setPlayer(Player* theplayer) {
    player = theplayer;
    // 设置玩家位置和摄像机位置一致
    if (auto camera = Director::getInstance()->getRunningScene()->getDefaultCamera()) {
        auto playerPos = player->getPosition3D();
        destination = playerPos;
        camera->setPosition3D(cocos2d::Vec3(playerPos.x, playerPos.y, camera->getPosition3D().z));
    }
}

void KeyboardEventManager::setNearestEnemy(Enemy* enemy) {
    nearestEnemy = enemy;
}

void KeyboardEventManager::setBackpackActive(bool active) {
    isBackpackActive = active;  // 设置背包界面是否激活
}

KeyboardEventManager::~KeyboardEventManager() {
    if (defaultEnemy) {
        delete defaultEnemy;
        defaultEnemy = nullptr;
    }
}