#ifndef BACKPACK_MAIN_LAYER_H
#define BACKPACK_MAIN_LAYER_H

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "BackpackLayer.h"
#include "SkillLayer.h"
#include "EquipmentLayer.h"
#include "../../Common/Entities/Player/Player.h"

class BackpackMainLayer : public cocos2d::Layer {
public:
    BackpackMainLayer();
    virtual ~BackpackMainLayer();

    // 创建主背包Layer并传入玩家数据
    static BackpackMainLayer* create(Player* player);

    // 初始化主背包Layer及UI元素
    virtual bool init();

    // 生成实例的宏
    CREATE_FUNC(BackpackMainLayer);

    // 设置玩家指针
    void setPlayer(Player* player);

    // 获取玩家指针
    Player* getPlayer() const;

    // 切换到背包界面
    void switchToBackpackLayer();

    // 切换到技能界面
    void switchToSkillLayer();

    // 切换到装备界面
    void switchToEquipmentLayer();

    void adjustSizeForTransition();
private:
    // 玩家对象
    Player* player;

    // 背包界面Layer
    BackpackLayer* backpackLayer;

    // 技能界面Layer
    SkillLayer* skillLayer;

    // 装备界面Layer
    EquipmentLayer* equipmentLayer;

    // 切换到下一个界面按钮
    cocos2d::ui::Button* nextButton;

    // 切换到上一个界面按钮
    cocos2d::ui::Button* prevButton;

    // 更新玩家数据UI（经验、等级、体力等）
    void updatePlayerData();

    // 创建武器、盔甲、饰品格子的显示
    void createEquipmentGrid();

    // 创建技能栏格子的显示
    void createSkillGrid();

    // 创建玩家切换形象的箭头
    void createArrowButtons();

    // 创建玩家数据显示
    void createPlayerDataDisplay();
};

#endif // BACKPACK_MAIN_LAYER_H
