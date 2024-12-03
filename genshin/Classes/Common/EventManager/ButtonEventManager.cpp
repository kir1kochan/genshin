#include "ButtonEventManager.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

void ButtonEventManager::initialize() {
    // 初始化按钮并添加监听器
    auto button = cocos2d::ui::Button::create("button_normal.png", "button_pressed.png"); // 示例，按需要会修改函数
    button->setPosition(cocos2d::Vec2(100, 100));  // 设置按钮的位置
    button->addTouchEventListener(CC_CALLBACK_2(ButtonEventManager::onButtonClick, this));  // 添加点击事件监听
    cocos2d::Director::getInstance()->getRunningScene()->addChild(button);  // 添加按钮到场景
}

void ButtonEventManager::handleEvent() {
    // 按钮事件通过回调处理，不需要额外处理
}

void ButtonEventManager::onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
        CCLOG("Button clicked!");  // 输出按钮点击日志
    }
}
