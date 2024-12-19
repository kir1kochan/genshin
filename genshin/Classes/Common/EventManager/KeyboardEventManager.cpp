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
    // ������������¼�
}

// ���̰����¼�����
void KeyboardEventManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (player == nullptr || isBackpackActive) return;  // ����������漤������¼�

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
    if (player == nullptr || isBackpackActive) return;  // ����������漤����Ը���

    // ����Ŀ��λ��
    Vec3 currentPos = player->getPosition3D();
    Vec3 targetPos = currentPos;
    targetPos.x += moveDirection.x * moveSpeed * deltaTime;
    targetPos.y += moveDirection.y * moveSpeed * deltaTime;

    // ���Ŀ��λ�÷����仯����������
    if (destination != targetPos) {
        destination = targetPos;

        // ֹ֮ͣǰ�Ķ����������µ�ƽ���ƶ�����
        player->stopAllActions(); // �����������
        auto moveAction = MoveTo::create(deltaTime, Vec2(destination.x, destination.y)); // 0.2 �����ƶ���Ŀ��
        player->runAction(moveAction);

        // �����������
        if (auto camera = Director::getInstance()->getRunningScene()->getDefaultCamera()) {
            Vec3 cameraTargetPos = Vec3(destination.x, destination.y, camera->getPosition3D().z);
            camera->stopAllActions(); // ֹ֮ͣǰ���������
            auto cameraMoveAction = MoveTo::create(deltaTime, Vec3(cameraTargetPos.x, cameraTargetPos.y,  camera->getPosition3D().z)); // ��΢�ӳ�
            camera->runAction(cameraMoveAction);
        }
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

void KeyboardEventManager::setNearestEnemy(Enemy* enemy) {
    nearestEnemy = enemy;
}

void KeyboardEventManager::setBackpackActive(bool active) {
    isBackpackActive = active;  // ���ñ��������Ƿ񼤻�
}

KeyboardEventManager::~KeyboardEventManager() {
    if (defaultEnemy) {
        delete defaultEnemy;
        defaultEnemy = nullptr;
    }
}