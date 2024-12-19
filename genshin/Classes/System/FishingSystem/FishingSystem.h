#ifndef FISHINGSYSTEM_H
#define FISHINGSYSTEM_H

#include "cocos2d.h"
#include <functional>

class FishingSystem : public cocos2d::Node {
public:
    FishingSystem();
    virtual ~FishingSystem();

    // 启动钓鱼过程
    void startFishing(cocos2d::Scene* fishingScene);

    // 处理玩家按键输入
    void onKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 更新钓鱼状态
    void update(float deltaTime, cocos2d::Scene* fishingScene);

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

    // 标记钓鱼是否正在进行
    bool fishingInProgress;

    // 用于绘制进度条的节点
    cocos2d::DrawNode* progressBarNode;
    cocos2d::DrawNode* greenBarNode;

    // UI更新所需的背景和条形图
    cocos2d::Sprite* backgroundBar;

    cocos2d::Label* fishingLabel;
};

#endif // FISHINGSYSTEM_H
