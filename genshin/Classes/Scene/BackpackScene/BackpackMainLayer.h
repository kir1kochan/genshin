#ifndef BACKPACK_MAIN_LAYER_H
#define BACKPACK_MAIN_LAYER_H

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "BackpackLayer.h"
#include "EquipmentLayer.h"
#include "../../Common/Entities/Player/Player.h"

class SkillLayer;

struct HoverInfo {
    bool isHovering;             // 是否正在悬停
    std::string name;            // 图标名称
    std::string effectValue;     // 效果
    std::function<void()> callback = nullptr;  // 可选的回调函数
};


namespace std {
    // 为 cocos2d::Rect 定义一个哈希函数
    template <>
    struct hash<cocos2d::Rect> {
        size_t operator()(const cocos2d::Rect& rect) const {
            size_t h1 = std::hash<float>{}(rect.origin.x);
            size_t h2 = std::hash<float>{}(rect.origin.y);
            size_t h3 = std::hash<float>{}(rect.size.width);
            size_t h4 = std::hash<float>{}(rect.size.height);

            // 使用组合哈希函数
            return h1 ^ h2 ^ h3 ^ h4;
        }
    };

    // 为 cocos2d::Rect 定义相等比较操作符
    template <>
    struct equal_to<cocos2d::Rect> {
        bool operator()(const cocos2d::Rect& lhs, const cocos2d::Rect& rhs) const {
            return lhs.origin.x == rhs.origin.x &&
                lhs.origin.y == rhs.origin.y &&
                lhs.size.width == rhs.size.width &&
                lhs.size.height == rhs.size.height;
        }
    };
}

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

    // 鼠标悬停监听器
    void addHoverListenerForIcons(Sprite* icon, const std::string& name, const std::string& effectValue, int id, std::function<void()> cb = nullptr);
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

    // 分成四个区域的无序图，每个无序图存储对应区域的 Rect 和 HoverInfo
    std::unordered_map<cocos2d::Rect, HoverInfo> topLeftGrid;     // 左上区域
    std::unordered_map<cocos2d::Rect, HoverInfo> topRightGrid;    // 右上区域
    std::unordered_map<cocos2d::Rect, HoverInfo> bottomLeftGrid;  // 左下区域
    std::unordered_map<cocos2d::Rect, HoverInfo> bottomRightGrid; // 右下区域
    int mouseArea; // 用于指示鼠标在哪个区域

    float gaptime = 0.0f; // 用于定时器计时

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

    // 装备信息显示器和技能信息显示器
    void showHoverInfo(const std::string& name, const std::string& effectValue, const Vec2& position);

    // 隐藏悬停信息
    void hideHoverInfo();

    void update(float deltatime);

    // 添加鼠标悬停监听
    void addHoverListener();
};

#endif // BACKPACK_MAIN_LAYER_H
