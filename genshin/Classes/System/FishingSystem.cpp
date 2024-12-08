#include "FishingSystem.h"
#include "cocos2d.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;

FishingSystem::FishingSystem()
    : fishingSuccessTime(0.0f), elapsedTime(0.0f), keyPressed(false) {
    // �����ʼ��
    srand(static_cast<unsigned>(time(0)));
}

FishingSystem::~FishingSystem() {
}

void FishingSystem::startFishing() {
    // ���ȷ��һ������ɹ���ʱ����������3�뵽5��֮�䣩
    fishingSuccessTime = static_cast<float>(rand() % 3 + 3);  // �ɹ�ʱ����3�뵽5��֮��
    elapsedTime = 0.0f;
    keyPressed = false;

    // ��������ʱ����ʾ����ҿ��԰��¿ո�����в���
    CCLOG("Fishing started! Press space bar within %.2f seconds to catch fish.", fishingSuccessTime);

    // ÿ֡���µ���״̬
    schedule([this](float deltaTime) {
        this->update(deltaTime);
        }, 0.1f, "fishing_update_key");

    // ���������¼�
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(FishingSystem::onKeyPress, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FishingSystem::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event) {
    // �����ո��������������Ӧ
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE && !keyPressed) {
        keyPressed = true;

        // ����Ƿ񰴵�����Ч��ʱ�����
        if (elapsedTime >= fishingSuccessTime - 0.5f && elapsedTime <= fishingSuccessTime + 0.5f) {
            CCLOG("Fishing successful! You caught the fish!");
            if (fishingResultCallback) {
                fishingResultCallback(true);  // ���óɹ��ص�
            }
        }
        else {
            CCLOG("Fishing failed! You missed the time window.");
            if (fishingResultCallback) {
                fishingResultCallback(false);  // ����ʧ�ܻص�
            }
        }
    }
}

void FishingSystem::update(float deltaTime) {
    elapsedTime += deltaTime;

    // �������ʱ�䴰�ڣ���û���¿ո�����Զ��ж�ʧ��
    if (elapsedTime > fishingSuccessTime + 1.0f && !keyPressed) {
        CCLOG("Fishing failed! You didn't press the key in time.");
        if (fishingResultCallback) {
            fishingResultCallback(false);  // ����ʧ�ܻص�
        }
        // ��������
        unschedule("fishing_update_key");
    }
}

void FishingSystem::setOnFishingResultCallback(std::function<void(bool)> callback) {
    fishingResultCallback = callback;
}
