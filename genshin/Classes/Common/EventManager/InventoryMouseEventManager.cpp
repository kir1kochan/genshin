#include "InventoryMouseEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void InventoryMouseEventManager::initialize() {
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(InventoryMouseEventManager::onMouseScroll, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void InventoryMouseEventManager::handleEvent() {
    // ��������¼��߼����绬�����ڣ�
}

void InventoryMouseEventManager::onMouseScroll(cocos2d::Event* event) {
    // �����������¼������ڻ�������
    auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
    float deltaY = mouseEvent->getScrollY();
    auto director = cocos2d::Director::getInstance();
    Camera* camera = Camera::getDefaultCamera();

    // ���ӻ�����������Z������ģ������
    Vec3 currentPos = camera->getPosition3D();
    if (deltaY > 0 && currentPos.y >= 400) {
        // ��ǰ�ӽ�����
        currentPos.y -= 20;
    }
    else if (deltaY < 0 && currentPos.y <= 800) {
        // ����ӽ�����
        currentPos.y += 20;
    }  // �������������
    camera->setPosition3D(currentPos);
}
