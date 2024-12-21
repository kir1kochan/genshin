#include "FishingSystem.h"
#include "cocos2d.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;

FishingSystem::FishingSystem()
    : fishingSuccessTime(0.0f), elapsedTime(0.0f), keyPressed(false), fishingInProgress(false) {
    srand(static_cast<unsigned>(time(0)));  // 随机初始化

    auto listener = cocos2d::EventListenerCustom::create("FISHING_ENDED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Fishing event ended!");
        if (!fishingInProgress) {
            return;
        }

        this->removeFromParent();
        this->removeAllChildren();
        if (backgroundBar) {
            backgroundBar = nullptr;
        }
        if (progressBarNode) {
            progressBarNode = nullptr;
        }
        if (fishingLabel) {
            fishingLabel = nullptr;
        }
        fishingInProgress = false;
        this->setVisible(false);
        unschedule("fishing_update_key");
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // 将监听器添加到事件分发器
}

FishingSystem::~FishingSystem() {
    unschedule("fishing_update_key");
}

void FishingSystem::startFishing(Scene* fishingScene) {
    if (fishingInProgress) return;  // 防止多次开始
    this->setVisible(true);
    fishingInProgress = true;
    fishingSuccessTime = static_cast<float>(rand() % 3 + 5);  // 成功时机刷新在3秒到7秒之间
    elapsedTime = 0.0f;
    keyPressed = false;

    // 创建UI：钓鱼时间条背景
    auto backgroundBar = Sprite::create("Backgrounds/Fishing_Background.jpg");
    Camera* camera = Camera::getDefaultCamera();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 获取相机位置，并设置背景条的目标位置
    Vec3 cameraPosition = camera->getPosition3D();
    Vec3 targetPosition = cameraPosition - Vec3(0, 0, 1100);  // 深度调整
    backgroundBar->setPosition3D(targetPosition); // 设置背景条位置
    this->addChild(backgroundBar);
    fishingScene->addChild(this, 9);
    // 设置背景条锚点为中心
    backgroundBar->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 创建DrawNode：绘制进度条背景（红色区域）
    progressBarNode = DrawNode::create();
    backgroundBar->addChild(progressBarNode);
    progressBarNode->setPosition(Vec2(backgroundBar->getContentSize().width / 2, 870));  // 居中显示

    // 绘制红色背景矩形
    progressBarNode->drawSolidRect(Vec2(-400, -30), Vec2(400, 30), Color4F::RED);

    // 创建DrawNode：绿色矩形（动态移动）
    greenBarNode = DrawNode::create();
    backgroundBar->addChild(greenBarNode);
    greenBarNode->setPosition(progressBarNode->getPosition());

    // 初始化绿色矩形位置，与红色矩形右端对齐
    float greenBarWidth = 80.0f;  // 假设绿色矩形宽度
    greenBarNode->drawSolidRect(Vec2(400 - greenBarWidth, -30), Vec2(400, 30), Color4F::GREEN);

    // 计算绿色矩形的移动速率
    float totalTime = 10.0f;  // 总时间
    float greenBarSpeed = 800.0f / fishingSuccessTime;  // 红色矩形宽度为800，计算每秒移动速度

    float successStart = fishingSuccessTime;  // 成功开始时间
    float successEnd = fishingSuccessTime + 1.0f;    // 成功结束时间

    CCLOG("Fishing started! Press space bar between %.2f and %.2f seconds.", successStart, successEnd);

    // 每帧更新钓鱼状态
    schedule([this, greenBarSpeed, successStart, successEnd](float deltaTime) {
        elapsedTime += deltaTime;

        // 更新绿色矩形位置
        float greenBarLeft = 400 - greenBarSpeed * elapsedTime;
        float greenBarRight = greenBarLeft + greenBarSpeed * 1.0f;
        if (greenBarRight > 400) {
            greenBarRight = 400;
        }
        if (greenBarLeft < -400) {
            greenBarLeft = -400;
        }
        if (greenBarRight < -400) {
            greenBarRight = -400;
        }
        // 清空之前的绘制内容
        greenBarNode->clear();
        greenBarNode->drawSolidRect(Vec2(greenBarLeft, -30), Vec2(greenBarRight, 30), Color4F::GREEN);

        // 检查钓鱼成功的时机
        if (elapsedTime >= 10.0f && !keyPressed) {
            CCLOG("Fishing failed! You didn't press the key in time.");
            if (fishingResultCallback) {
                fishingResultCallback(false);  // 调用失败回调
                fishingLabel->setString("Fishing failed! You missed the time window.");
            }
            unschedule("fishing_update_key");
        }
        }, 0.016f, "fishing_update_key");

    // 监听按键事件
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(FishingSystem::onKeyPress, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, fishingScene);
    // 创建 Label 显示钓鱼规则
    fishingLabel = Label::createWithTTF("Press the space bar when the green bar reaches the left edge.", "fonts/arial.ttf", 30);
    fishingLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);  // 居中显示
    fishingLabel->setTextColor(Color4B::WHITE);  // 设置文字颜色为白色
    fishingLabel->setName("fishingLabel");
    backgroundBar->addChild(fishingLabel, 10);  // 添加到场景中，zOrder 为 10（确保在最上层）
}

void FishingSystem::onKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE && !keyPressed) {
        keyPressed = true;

        // 检查是否按得在有效的时间段内
        float successStart = fishingSuccessTime;
        float successEnd = fishingSuccessTime + 1.0f;

        if (elapsedTime >= successStart && elapsedTime <= successEnd) {
            CCLOG("Fishing successful! You caught the fish!");
            if (fishingResultCallback) {
                fishingResultCallback(true);  // 调用成功回调
                fishingLabel->setString("Fishing successful! You caught the fish!");
            }
        }
        else {
            CCLOG("Fishing failed! You missed the time window.");
            if (fishingResultCallback) {
                fishingResultCallback(false);  // 调用失败回调
                fishingLabel->setString("Fishing failed! You missed the time window.");
            }
        }
        unschedule("fishing_update_key");
    }
}

void FishingSystem::setOnFishingResultCallback(std::function<void(bool)> callback) {
    fishingResultCallback = callback;
}

