#include "KeyboardEventManager.h"
#include "cocos2d.h"
USING_NS_CC;

void KeyboardEventManager::initialize() {
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyboardEventManager::onKeyPressed, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void KeyboardEventManager::handleEvent() {
    // ������������¼�
}

// ���̰����¼�����
void KeyboardEventManager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (player == nullptr) return;  // ȷ����Ҷ������

    cocos2d::Vec2 currentPos = player->getPosition();  // ��ȡ��ǰ���λ��
    cocos2d::Vec2 targetPos = currentPos;  // ��ʼĿ��λ���뵱ǰλ����ͬ

    // ���ݰ����ı�Ŀ��λ��
    switch (keyCode) {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:  // ����
        targetPos.y += moveSpeed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_S:  // ����
        targetPos.y -= moveSpeed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_A:  // ����
        targetPos.x -= moveSpeed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_D:  // ����
        targetPos.x += moveSpeed;
        break;
    default:
        break;
    }

    // ����Ŀ��λ��
    destination = targetPos;
}

// ���̰����ͷ��¼�����
void KeyboardEventManager::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    // �����ڰ����ͷ�ʱִ�ж���Ĳ�������ֹͣ�ƶ���
}

// ÿ֡�������λ��
void KeyboardEventManager::update(float deltaTime) {
    if (player == nullptr) return;  // ȷ����Ҷ������

    cocos2d::Vec2 currentPos = player->getPosition();  // ��ȡ��ǰ���λ��

    // ʹ�� Lerp ƽ���ع��ɵ�Ŀ��λ��
    cocos2d::Vec2 newPos = currentPos.lerp(destination, moveSpeed * deltaTime);
    player->setPosition(newPos);  // ��������µ�λ��
}

void KeyboardEventManager::setPlayer(Player* theplayer) {
    player = theplayer;
}