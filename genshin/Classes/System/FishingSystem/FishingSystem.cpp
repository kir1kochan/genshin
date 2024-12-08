#include "FishingSystem.h"
#include "cocos2d.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;

FishingSystem::FishingSystem()
    : fishingSuccessTime(0.0f), elapsedTime(0.0f), keyPressed(false) {
    // 随机初始化
    srand(static_cast<unsigned>(time(0)));
}

FishingSystem::~FishingSystem() {
}

void FishingSystem::startFishing() {
    // 随机确定一个钓鱼成功的时机（比如在3秒到5秒之间）
    fishingSuccessTime = static_cast<float>(rand() % 3 + 3);  // 成功时机在3秒到5秒之间
    elapsedTime = 0.0f;
    keyPressed = false;

    // 启动钓鱼时的提示，玩家可以按下空格键进行操作
    CCLOG("Fishing started! Press space bar within %.2f seconds to catch fish.", fishingSuccessTime);

    // 每帧更新钓鱼状态
    schedule([this](float deltaTime) {
        this->update(deltaTime);
        }, 0.1f, "fishing_update_key");

    // 监听按键事件
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(FishingSystem::onKeyPress, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FishingSystem::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event) {
    // 监听空格键，其他键不响应
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE && !keyPressed) {
        keyPressed = true;

        // 检查是否按得在有效的时间段内
        if (elapsedTime >= fishingSuccessTime - 0.5f && elapsedTime <= fishingSuccessTime + 0.5f) {
            CCLOG("Fishing successful! You caught the fish!");
            if (fishingResultCallback) {
                fishingResultCallback(true);  // 调用成功回调
            }
        }
        else {
            CCLOG("Fishing failed! You missed the time window.");
            if (fishingResultCallback) {
                fishingResultCallback(false);  // 调用失败回调
            }
        }
    }
}

void FishingSystem::update(float deltaTime) {
    elapsedTime += deltaTime;

    // 如果超过时间窗口，还没按下空格键，自动判定失败
    if (elapsedTime > fishingSuccessTime + 1.0f && !keyPressed) {
        CCLOG("Fishing failed! You didn't press the key in time.");
        if (fishingResultCallback) {
            fishingResultCallback(false);  // 调用失败回调
        }
        // 结束钓鱼
        unschedule("fishing_update_key");
    }
}

void FishingSystem::setOnFishingResultCallback(std::function<void(bool)> callback) {
    fishingResultCallback = callback;
}
