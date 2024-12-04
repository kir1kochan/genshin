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
    // ������������¼�
}

// ���̰����¼�����
void KeyboardEventManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (player == nullptr) return;  // ȷ����Ҷ������

    // �ڰ���ĳ������ʱ����¼�ð����ķ���
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        moveDirection.y = 1;  // ����
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        moveDirection.y = -1;  // ����
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        moveDirection.x = -1;  // ����
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        moveDirection.x = 1;   // ����
    }
}

// ���̰����ͷ��¼�����
void KeyboardEventManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        moveDirection.y = 0;  // ֹͣ����
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        moveDirection.y = 0;  // ֹͣ����
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        moveDirection.x = 0;  // ֹͣ����
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        moveDirection.x = 0;  // ֹͣ����
    }
}

// ÿ֡�������λ��
void KeyboardEventManager::update(float deltaTime) {
    if (player == nullptr) return;  // ȷ����Ҷ������

    Vec3 currentPos = player->getPosition3D();  // ��ȡ��ǰ���λ��
    Vec3 targetPos = currentPos;  // Ĭ��Ŀ��λ��Ϊ��ǰ���λ��

    // ���ݰ���״̬����Ŀ��λ��
    targetPos.x += moveDirection.x * moveSpeed * deltaTime;
    targetPos.y += moveDirection.y * moveSpeed * deltaTime;

    // ����Ŀ��λ��
    destination = targetPos;

    // ʹ�� Lerp ƽ���ع��ɵ�Ŀ��λ��
    // ����ƽ��
    Vec3 newPos = currentPos.lerp(destination, moveSpeed * deltaTime);
    player->setPosition3D(newPos);  // ��������µ�λ��

    // �������λ��
    if (auto camera = Director::getInstance()->getRunningScene()->getDefaultCamera()) {
        camera->setPosition3D(cocos2d::Vec3(newPos.x, newPos.y, camera->getPosition3D().z));
    }
}

void KeyboardEventManager::setPlayer(Player* theplayer) {
    player = theplayer;
    // �������λ�ú������λ��һ��
    if (auto camera = Director::getInstance()->getRunningScene()->getDefaultCamera()) {
        auto playerPos = player->getPosition3D();
        destination = playerPos;
        camera->setPosition3D(cocos2d::Vec3(playerPos.x, playerPos.y, camera->getPosition3D().z));
    }
}
