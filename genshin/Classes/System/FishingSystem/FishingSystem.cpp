#include "FishingSystem.h"
#include "cocos2d.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;

FishingSystem::FishingSystem()
    : fishingSuccessTime(0.0f), elapsedTime(0.0f), keyPressed(false), fishingInProgress(false) {
    srand(static_cast<unsigned>(time(0)));  // �����ʼ��

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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // ����������ӵ��¼��ַ���
}

FishingSystem::~FishingSystem() {
    unschedule("fishing_update_key");
}

void FishingSystem::startFishing(Scene* fishingScene) {
    if (fishingInProgress) return;  // ��ֹ��ο�ʼ
    this->setVisible(true);
    fishingInProgress = true;
    fishingSuccessTime = static_cast<float>(rand() % 3 + 5);  // �ɹ�ʱ��ˢ����3�뵽7��֮��
    elapsedTime = 0.0f;
    keyPressed = false;

    // ����UI������ʱ��������
    auto backgroundBar = Sprite::create("Backgrounds/Fishing_Background.jpg");
    Camera* camera = Camera::getDefaultCamera();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ��ȡ���λ�ã������ñ�������Ŀ��λ��
    Vec3 cameraPosition = camera->getPosition3D();
    Vec3 targetPosition = cameraPosition - Vec3(0, 0, 1100);  // ��ȵ���
    backgroundBar->setPosition3D(targetPosition); // ���ñ�����λ��
    this->addChild(backgroundBar);
    fishingScene->addChild(this, 9);
    // ���ñ�����ê��Ϊ����
    backgroundBar->setAnchorPoint(Vec2(0.5f, 0.5f));

    // ����DrawNode�����ƽ�������������ɫ����
    progressBarNode = DrawNode::create();
    backgroundBar->addChild(progressBarNode);
    progressBarNode->setPosition(Vec2(backgroundBar->getContentSize().width / 2, 870));  // ������ʾ

    // ���ƺ�ɫ��������
    progressBarNode->drawSolidRect(Vec2(-400, -30), Vec2(400, 30), Color4F::RED);

    // ����DrawNode����ɫ���Σ���̬�ƶ���
    greenBarNode = DrawNode::create();
    backgroundBar->addChild(greenBarNode);
    greenBarNode->setPosition(progressBarNode->getPosition());

    // ��ʼ����ɫ����λ�ã����ɫ�����Ҷ˶���
    float greenBarWidth = 80.0f;  // ������ɫ���ο��
    greenBarNode->drawSolidRect(Vec2(400 - greenBarWidth, -30), Vec2(400, 30), Color4F::GREEN);

    // ������ɫ���ε��ƶ�����
    float totalTime = 10.0f;  // ��ʱ��
    float greenBarSpeed = 800.0f / fishingSuccessTime;  // ��ɫ���ο��Ϊ800������ÿ���ƶ��ٶ�

    float successStart = fishingSuccessTime;  // �ɹ���ʼʱ��
    float successEnd = fishingSuccessTime + 1.0f;    // �ɹ�����ʱ��

    CCLOG("Fishing started! Press space bar between %.2f and %.2f seconds.", successStart, successEnd);

    // ÿ֡���µ���״̬
    schedule([this, greenBarSpeed, successStart, successEnd](float deltaTime) {
        elapsedTime += deltaTime;

        // ������ɫ����λ��
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
        // ���֮ǰ�Ļ�������
        greenBarNode->clear();
        greenBarNode->drawSolidRect(Vec2(greenBarLeft, -30), Vec2(greenBarRight, 30), Color4F::GREEN);

        // ������ɹ���ʱ��
        if (elapsedTime >= 10.0f && !keyPressed) {
            CCLOG("Fishing failed! You didn't press the key in time.");
            if (fishingResultCallback) {
                fishingResultCallback(false);  // ����ʧ�ܻص�
                fishingLabel->setString("Fishing failed! You missed the time window.");
            }
            unschedule("fishing_update_key");
        }
        }, 0.016f, "fishing_update_key");

    // ���������¼�
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(FishingSystem::onKeyPress, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, fishingScene);
    // ���� Label ��ʾ�������
    fishingLabel = Label::createWithTTF("Press the space bar when the green bar reaches the left edge.", "fonts/arial.ttf", 30);
    fishingLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);  // ������ʾ
    fishingLabel->setTextColor(Color4B::WHITE);  // ����������ɫΪ��ɫ
    fishingLabel->setName("fishingLabel");
    backgroundBar->addChild(fishingLabel, 10);  // ��ӵ������У�zOrder Ϊ 10��ȷ�������ϲ㣩
}

void FishingSystem::onKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE && !keyPressed) {
        keyPressed = true;

        // ����Ƿ񰴵�����Ч��ʱ�����
        float successStart = fishingSuccessTime;
        float successEnd = fishingSuccessTime + 1.0f;

        if (elapsedTime >= successStart && elapsedTime <= successEnd) {
            CCLOG("Fishing successful! You caught the fish!");
            if (fishingResultCallback) {
                fishingResultCallback(true);  // ���óɹ��ص�
                fishingLabel->setString("Fishing successful! You caught the fish!");
            }
        }
        else {
            CCLOG("Fishing failed! You missed the time window.");
            if (fishingResultCallback) {
                fishingResultCallback(false);  // ����ʧ�ܻص�
                fishingLabel->setString("Fishing failed! You missed the time window.");
            }
        }
        unschedule("fishing_update_key");
    }
}

void FishingSystem::setOnFishingResultCallback(std::function<void(bool)> callback) {
    fishingResultCallback = callback;
}

