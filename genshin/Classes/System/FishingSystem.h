#ifndef FISHINGSYSTEM_H
#define FISHINGSYSTEM_H

#include "cocos2d.h"

class FishingSystem : public cocos2d::Node {
public:
    FishingSystem();
    virtual ~FishingSystem();

    // 启动钓鱼过程
    void startFishing();

    // 处理玩家按键输入
    void onKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 更新钓鱼状态
    void update(float deltaTime);

    // 设置钓鱼成功后执行的回调
    void setOnFishingResultCallback(std::function<void(bool)> callback);

private:
    // 钓鱼时机（成功的时机：以秒为单位）
    float fishingSuccessTime;

    // 当前时间（以秒为单位）
    float elapsedTime;

    // 是否成功按下按键
    bool keyPressed;

    // 回调函数，返回钓鱼结果
    std::function<void(bool)> fishingResultCallback;
};

#endif // FISHINGSYSTEM_H
