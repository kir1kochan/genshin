#include "KeyboardEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void KeyboardEventManager::initialize() {
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyboardEventManager::onKeyPressed, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void KeyboardEventManager::handleEvent() {
    // 处理键盘输入事件
}

// 键盘按下事件处理
void KeyboardEventManager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (player == nullptr) return;  // 确保玩家对象存在

    cocos2d::Vec2 currentPos = player->getPosition();  // 获取当前玩家位置
    cocos2d::Vec2 targetPos = currentPos;  // 初始目标位置与当前位置相同

    // 根据按键改变目标位置
    switch (keyCode) {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:  // 向上
        targetPos.y += moveSpeed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_S:  // 向下
        targetPos.y -= moveSpeed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_A:  // 向左
        targetPos.x -= moveSpeed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_D:  // 向右
        targetPos.x += moveSpeed;
        break;
    default:
        break;
    }

    // 设置目标位置
    destination = targetPos;
}

// 键盘按键释放事件处理
void KeyboardEventManager::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    // 可以在按键释放时执行额外的操作（如停止移动）
}

// 每帧更新玩家位置
void KeyboardEventManager::update(float deltaTime) {
    if (player == nullptr) return;  // 确保玩家对象存在

    cocos2d::Vec2 currentPos = player->getPosition();  // 获取当前玩家位置

    // 使用 Lerp 平滑地过渡到目标位置
    cocos2d::Vec2 newPos = currentPos.lerp(destination, moveSpeed * deltaTime);
    player->setPosition(newPos);  // 设置玩家新的位置
}

void KeyboardEventManager::setPlayer(Player* theplayer) {
    player = theplayer;
}