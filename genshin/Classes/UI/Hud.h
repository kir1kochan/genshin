#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"

class Player;

class Hud : public cocos2d::Node {
private:
    // ���������ƽڵ�
    cocos2d::DrawNode* healthBarNode;  // Ѫ���ڵ�
    cocos2d::DrawNode* staminaBarNode; // �������ڵ�
    cocos2d::TMXTiledMap* miniMapNode; // С��ͼ�ڵ�
    cocos2d::Sprite* miniMapPlayerIcon; // С��ͼ���ͼ��
    cocos2d::ClippingNode* clipper;//�ü�
    cocos2d::DrawNode* stencil;//���ֲ�
    cocos2d::DrawNode* borderNode;//�߿�
    cocos2d::TMXTiledMap* expandedMiniMapNode ;
    cocos2d::Sprite* expandedMiniMapPlayerIcon;

    // �������Ŀ�Ⱥ͸߶�
    float barWidth;  // ���������ܿ��
    float barHeight; // �������ĸ߶�

    // ������Ҷ���
    Player* player;

    // С��ͼ��ʾ״̬
    bool isMiniMapExpanded;

    // ���½�����λ�ã������ص���
    void updateBarPositions();

    // ����ͨ�õľ��ν�����
    void drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color);

    // ��ʼ��С��ͼ���ͼ��
    void initMiniMapPlayerIcon();

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

    // ����С��ͼ��λ��
    void updateMiniMapPosition();
    //�л�С��ͼ��ʾ��ʽ
    void toggleMiniMap();
};

#endif // HUD_H