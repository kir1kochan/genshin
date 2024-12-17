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

    cocos2d::Node* equipmentIconsContainer; // 存储装备图标的容器

    cocos2d::Node* playerInfoContainer; // 存储装备图标的容器

    cocos2d::Node* skillIconsContainer; // 存储装备图标的容器

    Label* _hoverLabel = nullptr;  // 用于显示装备着的装备和技能描述的标签

    Layer* _hoverLabelBackground = nullptr; // 标签的背景

    // 创建碰撞盒
    void createBoundingBoxForIcons(cocos2d::Sprite* sprite);

    // 更新玩家数据UI（经验、等级、体力等）
    void updatePlayerData();

    // 创建武器、盔甲、饰品格子的显示
    void createEquipmentGrid();

    // 创建技能栏格子的显示
    void createSkillGrid();

    // 创建背包格子的显示
    void createBackpackUI();

    // 创建玩家切换形象的箭头
    void createArrowButtons();

    // 创建玩家数据显示
    void createPlayerDataDisplay();

    // 生成装备图标
    void createEquipmentIcons();

    // 双击事件监听器
    void addDoubleClickListener(cocos2d::Sprite* target, const std::function<void()>& callback);

    // 刷新装备图标
    void refreshEquipmentIcons();

    // 根据按钮切换角色属性和外观
    void onLeftArrowClicked();
    void onRightArrowClicked();

    // 鼠标悬停监听器
    void addHoverListenerForIcons(Sprite* icon, const std::string& name, const std::string& effectValue, int id);

    // 装备信息显示器和技能信息显示器
    void showHoverInfo(const std::string& name, const std::string& effectValue, const Vec2& position);
    // 隐藏悬停信息
    void hideHoverInfo()
};

#endif // BACKPACK_MAIN_LAYER_H
