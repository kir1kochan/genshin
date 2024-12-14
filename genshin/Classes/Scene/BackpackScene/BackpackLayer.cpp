#include "BackpackLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

BackpackLayer::BackpackLayer() {}

BackpackLayer::~BackpackLayer() {}

bool BackpackLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // 创建背包界面的UI组件
    createBackpackUI();

    return true;
}

void BackpackLayer::createBackpackUI() {
    // 创建一个标签显示背包界面的标题
    auto label = Label::createWithSystemFont("Backpack", "Arial", 24);
    label->setPosition(Vec2(300, 400));
    this->addChild(label);

    // 这里可以根据需求继续添加其他UI元素，如物品格子、物品图标等
}
