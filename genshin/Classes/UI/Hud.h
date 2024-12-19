#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"

class Player;

class Hud : public cocos2d::Node {
private:
    // 进度条绘制节点
    cocos2d::DrawNode* healthBarNode;  // 血条节点
    cocos2d::DrawNode* staminaBarNode; // 体力条节点

    // 进度条的宽度和高度
    float barWidth;  // 进度条的总宽度
    float barHeight; // 进度条的高度

    // 引用玩家对象
    Player* player;

    // 更新进度条位置（避免重叠）
    void updateBarPositions();

    // 绘制通用的矩形进度条
    void drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color);

public:
    // 构造函数与析构函数
    Hud(Player* player);
    virtual ~Hud();

    // 静态创建方法
    static Hud* create(Player* player);

    // 每帧更新方法
    void update(float deltaTime);

    // 更新血条和体力条
    void updateHealthBar(float healthPercent);
    void updateStaminaBar(float staminaPercent);
};

#endif // HUD_H