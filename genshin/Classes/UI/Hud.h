#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"
#include <vector>

class TPAnchor;
class Player;

class Hud : public cocos2d::Node {
private:
    // 进度条绘制节点
    cocos2d::DrawNode* healthBarNode;  // 血条节点
    cocos2d::DrawNode* staminaBarNode; // 体力条节点
    cocos2d::TMXTiledMap* miniMapNode; // 小地图节点
    cocos2d::Sprite* miniMapPlayerIcon; // 小地图玩家图标
    cocos2d::ClippingNode* clipper; // 裁剪
    cocos2d::DrawNode* stencil; // 遮罩层
    cocos2d::DrawNode* borderNode; // 边框
    cocos2d::TMXTiledMap* expandedMiniMapNode;
    cocos2d::Sprite* expandedMiniMapPlayerIcon;

    // 技能栏绘制节点
    std::vector<cocos2d::DrawNode*> skillBarNode;   // 技能栏节点

    // 冷却罩子（进度条）
    std::vector<cocos2d::ProgressTimer*> skillCooldownBars;  // 冷却进度条

    // 进度条的宽度和高度
    float barWidth;  // 进度条的总宽度
    float barHeight; // 进度条的高度

    TPAnchor* tpAnchor; // 用于传送点
    Player* player;     // 引用玩家对象

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

    // 设置传送点
    void setTPAnchor(TPAnchor* tpAnchor);

    // 静态创建方法
    static Hud* create(Player* player);

    // 每帧更新方法
    void update(float deltaTime);

    // 更新血条和体力条
    void updateHealthBar(float healthPercent);
    void updateStaminaBar(float staminaPercent);

    // 装备技能
    void equipSkill(int skillSlot, std::string skillName);
    // 卸载技能
    void unequipSkill(int skillSlot);
    // 使用技能
    void useSkill(int skillSlot, float cdTime);

    // 更新小地图的位置
    void updateMiniMapPosition();
    // 切换小地图显示方式
    void toggleMiniMap();

    // 更新主线任务图标位置
    void updateMissionIconPosition(int stage);
    // 更新支线任务图标位置
    void updateSideMissionIconPosition(const std::string& missionName, bool isVisible);

    // 隐藏雾层
    void hideFogLayers(cocos2d::TMXTiledMap* map);
    void hideFogLayers(cocos2d::TMXTiledMap* map, int index);

    // 获取小地图节点
    cocos2d::TMXTiledMap* getMiniMapNode() const;
    // 获取扩展的小地图节点
    cocos2d::TMXTiledMap* getExpandedMiniMapNode() const;

    // 获取小地图显示状态
    bool getIsExpanded() const;

    // 设置悬浮小地图的可见性
    void setMiniMapVisible(bool visible);

    // 获取悬浮小地图中玩家的位置
    cocos2d::Vec2 getMiniMapPlayerPosition() const;

    // 设置悬浮小地图中玩家的位置
    void setMiniMapPlayerPosition(const cocos2d::Vec2& position);
};

#endif // HUD_H
