#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"

// 构造函数，初始化各成员变量
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f) {
    // 创建 DrawNode 用于显示血条和体力条
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // 将血条和体力条节点添加为子节点
    addChild(healthBarNode);
    addChild(staminaBarNode);

    // 设置初始 HUD 的位置
    setPosition(cocos2d::Vec2(0, 100));  // HUD 相对于玩家的顶部

    // 初始化进度条的相对位置
    updateBarPositions();
}

// 析构函数
Hud::~Hud() {
}

// 静态创建方法
Hud* Hud::create(Player* player) {
    Hud* hud = new (std::nothrow) Hud(player);
    if (hud && hud->init()) {
        hud->autorelease();
        return hud;
    }
    CC_SAFE_DELETE(hud);
    return nullptr;
}

// 每帧更新 HUD 的状态
void Hud::update(float deltaTime) {
    // 根据玩家当前状态更新血条和体力条
    updateHealthBar(player->getHealth() / player->getMaxHealth());
    updateStaminaBar(player->getStamina() / player->getMaxStamina());

    // 更新进度条的位置
    updateBarPositions();
}

// 更新血条显示
void Hud::updateHealthBar(float healthPercent) {
    drawProgressBar(healthBarNode, healthPercent, cocos2d::Color4F::RED);
}

// 更新体力条显示
void Hud::updateStaminaBar(float staminaPercent) {
    drawProgressBar(staminaBarNode, staminaPercent, cocos2d::Color4F::GREEN);
}

// 绘制进度条
void Hud::drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color) {
    barNode->clear(); // 清空旧的内容

    // 确保百分比在 0 到 1 之间
    percent = std::max(0.0f, std::min(1.0f, percent));
    float filledWidth = barWidth * percent;

    // 绘制矩形进度条
    barNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(filledWidth, barHeight), color);

    // 绘制边框
    cocos2d::Vec2 origin(0, 0); // 起点
    cocos2d::Vec2 topRight(barWidth, barHeight); // 右上角点
    barNode->drawRect(origin, topRight, cocos2d::Color4F::BLACK); // 黑色边框
}

// 设置进度条的相对位置
void Hud::updateBarPositions() {
    float offsetX = -barWidth - 300; // 距离玩家左侧的偏移量
    float offsetY = -375;           // 距离玩家底部的偏移量

    // 设置每个条的位置
    healthBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY + 30)); // 血条（稍上）
    staminaBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY));     // 体力条（稍下）
}