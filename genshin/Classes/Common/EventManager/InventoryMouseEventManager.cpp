#include "InventoryMouseEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void InventoryMouseEventManager::initialize() {
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(InventoryMouseEventManager::onMouseScroll, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void InventoryMouseEventManager::handleEvent() {
    // 处理鼠标事件逻辑（如滑动窗口）
}

void InventoryMouseEventManager::onMouseScroll(cocos2d::Event* event) {
    // 处理鼠标滚轮事件，用于滑动窗口
    auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
    float deltaY = mouseEvent->getScrollY();
    auto director = cocos2d::Director::getInstance();
    Camera* camera = Camera::getDefaultCamera();

    // 增加或减少摄像机的Z坐标来模拟缩放
    Vec3 currentPos = camera->getPosition3D();
    if (deltaY > 0 && currentPos.y >= 400) {
        // 向前视角向上
        currentPos.y -= 20;
    }
    else if (deltaY < 0 && currentPos.y <= 800) {
        // 向后视角向下
        currentPos.y += 20;
    }  // 按需更改上下限
    camera->setPosition3D(currentPos);
}
