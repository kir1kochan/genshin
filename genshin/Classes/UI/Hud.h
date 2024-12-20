#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"
#include<vector>

class Player;

class Hud : public cocos2d::Node {
private:
    // 进度条绘制节点
    cocos2d::DrawNode* healthBarNode;  // 血条节点
    cocos2d::DrawNode* staminaBarNode; // 体力条节点

    // 技能栏绘制节点
    std::vector<cocos2d::DrawNode*> skillBarNode;   // 技能栏节点

    // 冷却罩子（进度条）
    std::vector<cocos2d::ProgressTimer*> skillCooldownBars;  // 冷却进度条

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

    // 装备技能
    void equipSkill(int skillSlot, std::string skillName);
    // 卸载技能
    void unequipSkill(int skillSlot);

    void useSkill(int skillSlot, float cdTime);



};

#endif // HUD_H