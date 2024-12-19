#ifndef FISHINGSYSTEM_H
#define FISHINGSYSTEM_H

#include "cocos2d.h"
#include <functional>

class FishingSystem : public cocos2d::Node {
public:
    FishingSystem();
    virtual ~FishingSystem();

    // �����������
    void startFishing(cocos2d::Scene* fishingScene);

    // ������Ұ�������
    void onKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // ���µ���״̬
    void update(float deltaTime, cocos2d::Scene* fishingScene);

    // ���õ���ɹ���ִ�еĻص�
    void setOnFishingResultCallback(std::function<void(bool)> callback);

private:
    // ����ʱ�����ɹ���ʱ��������Ϊ��λ��
    float fishingSuccessTime;

    // ��ǰʱ�䣨����Ϊ��λ��
    float elapsedTime;

    // �Ƿ�ɹ����°���
    bool keyPressed;

    // �ص����������ص�����
    std::function<void(bool)> fishingResultCallback;

    // ��ǵ����Ƿ����ڽ���
    bool fishingInProgress;

    // ���ڻ��ƽ������Ľڵ�
    cocos2d::DrawNode* progressBarNode;
    cocos2d::DrawNode* greenBarNode;

    // UI��������ı���������ͼ
    cocos2d::Sprite* backgroundBar;

    cocos2d::Label* fishingLabel;
};

#endif // FISHINGSYSTEM_H
