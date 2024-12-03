#include "ButtonEventManager.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

void ButtonEventManager::initialize() {
    // ��ʼ����ť����Ӽ�����
    auto button = cocos2d::ui::Button::create("button_normal.png", "button_pressed.png"); // ʾ��������Ҫ���޸ĺ���
    button->setPosition(cocos2d::Vec2(100, 100));  // ���ð�ť��λ��
    button->addTouchEventListener(CC_CALLBACK_2(ButtonEventManager::onButtonClick, this));  // ��ӵ���¼�����
    cocos2d::Director::getInstance()->getRunningScene()->addChild(button);  // ��Ӱ�ť������
}

void ButtonEventManager::handleEvent() {
    // ��ť�¼�ͨ���ص���������Ҫ���⴦��
}

void ButtonEventManager::onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
        CCLOG("Button clicked!");  // �����ť�����־
    }
}
