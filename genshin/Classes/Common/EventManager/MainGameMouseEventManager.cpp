#include "MainGameMouseEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void MainGameMouseEventManager::initialize() {
    // ����_isListeningΪtrueʱ������¼�����
    if (_isListening) {
        auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseScroll = CC_CALLBACK_1(MainGameMouseEventManager::onMouseScroll, this);
        listener->onMouseDown = CC_CALLBACK_1(MainGameMouseEventManager::onMouseDown, this);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
    }
}

void MainGameMouseEventManager::handleEvent() {
    // ��������¼��߼������ӽ����ţ�
}

void MainGameMouseEventManager::onMouseDown(cocos2d::Event* event) {
    if (_isListening) {
        // ������갴���¼���������ӽ��ĵ���
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

        Vec3 hudPos = hud->getPosition3D();
        // ���ӻ�����������Z������ģ������
        Vec3 currentPos = camera->getPosition3D();
        if (deltaY > 0 && currentPos.z >= 400) {
            // ��ǰ��С
            currentPos.z -= 20;
            hudPos.z -= 20;
        }
        else if (deltaY < 0 && currentPos.z <= 800) {
            // ���Ŵ�
            currentPos.z += 20;
            hudPos.z += 20;
        }
        camera->setPosition3D(currentPos);
        hud->setPosition3D(hudPos);
    }
}

void MainGameMouseEventManager::setPlayer(Player* theplayer) {
    player = theplayer;
    hud = dynamic_cast<Hud*>(player->getChildByName("hud"));
    // �������λ�ú������λ��һ��
}

void MainGameMouseEventManager::setNearestEnemy(Enemy* enemy) {
    nearestEnemy = enemy;
}