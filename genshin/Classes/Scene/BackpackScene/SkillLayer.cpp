#include "SkillLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

SkillLayer::SkillLayer() {}

SkillLayer::~SkillLayer() {}

bool SkillLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // 创建技能界面的UI组件
    createSkillUI();

    return true;
}

void SkillLayer::createSkillUI() {
    // 创建一个标签显示技能界面的标题
    auto label = Label::createWithSystemFont("Skills", "Arial", 24);
    label->setPosition(Vec2(300, 100));
    this->addChild(label);

    // 物品格子的起始位置
    float startX = 120.0f;
    float startY = 185.0f;

    // 每行显示多少个格子
    int itemsPerRow = 5;
    float itemWidth = 90.0f;
    float itemHeight = 90.0f;

    // 创建并平铺格子
    for (int i = 0; i < 5; ++i) { // 假设你要创建 20 个格子
        // 计算当前格子的位置
        float x = startX + (i % itemsPerRow) * (itemWidth + 5); // 每个格子之间有 5 的间隔
        float y = startY - (i / itemsPerRow) * (itemHeight + 5); // 每行的格子间隔 5

        // 创建格子的 Sprite 并设置位置
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        this->addChild(itemSprite);
    }
    // 这里可以根据需求继续添加其他UI元素，如技能按钮、技能图标等
}
