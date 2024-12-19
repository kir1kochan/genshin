#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"

class Player;

class Hud : public cocos2d::Node {
private:
    // ���������ƽڵ�
    cocos2d::DrawNode* healthBarNode;  // Ѫ���ڵ�
    cocos2d::DrawNode* staminaBarNode; // �������ڵ�

    // �������Ŀ�Ⱥ͸߶�
    float barWidth;  // ���������ܿ��
    float barHeight; // �������ĸ߶�

    // ������Ҷ���
    Player* player;

    // ���½�����λ�ã������ص���
    void updateBarPositions();

    // ����ͨ�õľ��ν�����
    void drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color);

public:
    // ���캯������������
    Hud(Player* player);
    virtual ~Hud();

    // ��̬��������
    static Hud* create(Player* player);

    // ÿ֡���·���
    void update(float deltaTime);

    // ����Ѫ����������
    void updateHealthBar(float healthPercent);
    void updateStaminaBar(float staminaPercent);
};

#endif // HUD_H