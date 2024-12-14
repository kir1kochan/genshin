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
    label->setPosition(Vec2(300, 400));
    this->addChild(label);

    // 这里可以根据需求继续添加其他UI元素，如技能按钮、技能图标等
}
