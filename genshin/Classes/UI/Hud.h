#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"

class Player;

class Hud : public cocos2d::Node {
private:
    // 进度条绘制节点
    cocos2d::DrawNode* healthBarNode;  // 血条节点
    cocos2d::DrawNode* staminaBarNode; // 体力条节点
    cocos2d::TMXTiledMap* miniMapNode; // 小地图节点
    cocos2d::Sprite* miniMapPlayerIcon; // 小地图玩家图标
    cocos2d::ClippingNode* clipper;//裁剪
    cocos2d::DrawNode* stencil;//遮罩层
    cocos2d::DrawNode* borderNode;//边框
    cocos2d::TMXTiledMap* expandedMiniMapNode ;
    cocos2d::Sprite* expandedMiniMapPlayerIcon;

    // 进度条的宽度和高度
    float barWidth;  // 进度条的总宽度
    float barHeight; // 进度条的高度

    // 引用玩家对象
    Player* player;

    // 小地图显示状态
    bool isMiniMapExpanded;

    // 更新进度条位置（避免重叠）
    void updateBarPositions();

    // 绘制通用的矩形进度条
    void drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color);

    // 初始化小地图玩家图标
    void initMiniMapPlayerIcon();

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

    // 更新小地图的位置
    void updateMiniMapPosition();
    //切换小地图显示方式
    void toggleMiniMap();
};

#endif // HUD_H