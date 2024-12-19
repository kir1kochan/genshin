#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"

// ���캯������ʼ������Ա����
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f) {
    // ���� DrawNode ������ʾѪ����������
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // ��Ѫ�����������ڵ����Ϊ�ӽڵ�
    addChild(healthBarNode);
    addChild(staminaBarNode);

    // ���ó�ʼ HUD ��λ��
    setPosition(cocos2d::Vec2(0, 100));  // HUD �������ҵĶ���

    // ��ʼ�������������λ��
    updateBarPositions();
}

// ��������
Hud::~Hud() {
}

// ��̬��������
Hud* Hud::create(Player* player) {
    Hud* hud = new (std::nothrow) Hud(player);
    if (hud && hud->init()) {
        hud->autorelease();
        return hud;
    }
    CC_SAFE_DELETE(hud);
    return nullptr;
}

// ÿ֡���� HUD ��״̬
void Hud::update(float deltaTime) {
    // ������ҵ�ǰ״̬����Ѫ����������
    updateHealthBar(player->getHealth() / player->getMaxHealth());
    updateStaminaBar(player->getStamina() / player->getMaxStamina());

    // ���½�������λ��
    updateBarPositions();
}

// ����Ѫ����ʾ
void Hud::updateHealthBar(float healthPercent) {
    drawProgressBar(healthBarNode, healthPercent, cocos2d::Color4F::RED);
}

// ������������ʾ
void Hud::updateStaminaBar(float staminaPercent) {
    drawProgressBar(staminaBarNode, staminaPercent, cocos2d::Color4F::GREEN);
}

// ���ƽ�����
void Hud::drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color) {
    barNode->clear(); // ��վɵ�����

    // ȷ���ٷֱ��� 0 �� 1 ֮��
    percent = std::max(0.0f, std::min(1.0f, percent));
    float filledWidth = barWidth * percent;

    // ���ƾ��ν�����
    barNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(filledWidth, barHeight), color);

    // ���Ʊ߿�
    cocos2d::Vec2 origin(0, 0); // ���
    cocos2d::Vec2 topRight(barWidth, barHeight); // ���Ͻǵ�
    barNode->drawRect(origin, topRight, cocos2d::Color4F::BLACK); // ��ɫ�߿�
}

// ���ý����������λ��
void Hud::updateBarPositions() {
    float offsetX = -barWidth - 300; // �����������ƫ����
    float offsetY = -375;           // ������ҵײ���ƫ����

    // ����ÿ������λ��
    healthBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY + 30)); // Ѫ�������ϣ�
    staminaBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY));     // �����������£�
}