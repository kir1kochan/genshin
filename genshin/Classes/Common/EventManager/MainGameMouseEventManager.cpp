#include "MainGameMouseEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void MainGameMouseEventManager::initialize() {
    // 仅当_isListening为true时才添加事件监听
    if (_isListening) {
        auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseScroll = CC_CALLBACK_1(MainGameMouseEventManager::onMouseScroll, this);
        listener->onMouseDown = CC_CALLBACK_1(MainGameMouseEventManager::onMouseDown, this);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
    }
}

void MainGameMouseEventManager::handleEvent() {
    // 处理鼠标事件逻辑（如视角缩放）
}

void MainGameMouseEventManager::onMouseDown(cocos2d::Event* event) {
    if (_isListening) {
        // 处理鼠标按下事件，攻击最接近的敌人
        if (nearestEnemy) {
            player->attackTarget(*nearestEnemy);
        }
    }
}

void MainGameMouseEventManager::onMouseScroll(cocos2d::Event* event) {
    if (_isListening) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        float deltaY = mouseEvent->getScrollY();
        auto director = cocos2d::Director::getInstance();
        Camera* camera = Camera::getDefaultCamera();

        // 增加或减少摄像机的Z坐标来模拟缩放
        Vec3 currentPos = camera->getPosition3D();
        if (deltaY > 0 && currentPos.z >= 400) {
            // 向前缩小
            currentPos.z -= 20;
        }
        else if (deltaY < 0 && currentPos.z <= 800) {
            // 向后放大
            currentPos.z += 20;
        }
        camera->setPosition3D(currentPos);
    }
}

void MainGameMouseEventManager::setPlayer(Player* theplayer) {
    player = theplayer;
    // 设置玩家位置和摄像机位置一致
}

void MainGameMouseEventManager::setNearestEnemy(Enemy* enemy) {
    nearestEnemy = enemy;
}