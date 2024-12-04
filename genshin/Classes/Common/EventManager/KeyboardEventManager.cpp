#include "KeyboardEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void KeyboardEventManager::initialize() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyboardEventManager::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(KeyboardEventManager::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, Director::getInstance()->getRunningScene());
    moveDirection.y = 0;  
    moveDirection.x = 0;  
}

void KeyboardEventManager::handleEvent() {
    // 处理键盘输入事件
}

// 键盘按下事件处理
void KeyboardEventManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (player == nullptr) return;  // 确保玩家对象存在

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
void KeyboardEventManager::update(float deltaTime) {
    if (player == nullptr) return;  // 确保玩家对象存在

    Vec3 currentPos = player->getPosition3D();  // 获取当前玩家位置
    Vec3 targetPos = currentPos;  // 默认目标位置为当前玩家位置

    // 根据按键状态更新目标位置
    targetPos.x += moveDirection.x * moveSpeed * deltaTime;
    targetPos.y += moveDirection.y * moveSpeed * deltaTime;

    // 设置目标位置
    destination = targetPos;

    // 使用 Lerp 平滑地过渡到目标位置
    // 不够平滑
    Vec3 newPos = currentPos.lerp(destination, moveSpeed * deltaTime);
    player->setPosition3D(newPos);  // 设置玩家新的位置

    // 更新相机位置
    if (auto camera = Director::getInstance()->getRunningScene()->getDefaultCamera()) {
        camera->setPosition3D(cocos2d::Vec3(newPos.x, newPos.y, camera->getPosition3D().z));
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
