#ifndef FISHINGSYSTEM_H
#define FISHINGSYSTEM_H

#include "cocos2d.h"

class FishingSystem : public cocos2d::Node {
public:
    FishingSystem();
    virtual ~FishingSystem();

    // �����������
    void startFishing();

    // ������Ұ�������
    void onKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // ���µ���״̬
    void update(float deltaTime);

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
};

#endif // FISHINGSYSTEM_H
