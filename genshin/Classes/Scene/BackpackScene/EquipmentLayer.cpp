#include "EquipmentLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

EquipmentLayer::EquipmentLayer() {}

EquipmentLayer::~EquipmentLayer() {}

bool EquipmentLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // 创建装备界面的UI组件
    createEquipmentUI();

    return true;
}

void EquipmentLayer::createEquipmentUI() {
    // 创建一个标签显示装备界面的标题
    auto label = Label::createWithSystemFont("Equipment", "Arial", 24);
    label->setPosition(Vec2(300, 400));
    this->addChild(label);

    // 这里可以根据需求继续添加其他UI元素，如装备槽、技能按钮等
}
