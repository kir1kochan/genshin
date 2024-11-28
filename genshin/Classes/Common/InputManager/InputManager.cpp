#include "InputManager.h"
#include "cocos2d.h"
USING_NS_CC;

// MouseInputManager1 ʵ��
void MouseInputManager1::initialize() {
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(MouseInputManager1::onMouseScroll, this);
    listener->onMouseDown = CC_CALLBACK_1(MouseInputManager1::onMouseDown, this);  // ������갴���¼�
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void MouseInputManager1::handleInput() {
    // �������������߼�
}

void MouseInputManager1::onMouseDown(cocos2d::Event* event) {
    // ������갴���¼�
}

void MouseInputManager1::onMouseScroll(cocos2d::Event* event) {
    auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
    float deltaY = mouseEvent->getScrollY();
    auto director = cocos2d::Director::getInstance();
    Camera* camera = Camera::getDefaultCamera();

    // ���ӻ�����������Z������ģ������
    Vec3 currentPos = camera->getPosition3D();
    if (deltaY > 0 && currentPos.z >= 400) {
        // ��ǰ��С
        currentPos.z -= 20;
    }
    else if (deltaY < 0 && currentPos.z <= 800){
        // ���Ŵ�
        currentPos.z += 20;
    }
    camera->setPosition3D(currentPos);
}

// KeyboardInputManager ʵ��
void KeyboardInputManager::initialize() {
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyboardInputManager::onKeyPressed, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void KeyboardInputManager::handleInput() {
    // �����������
}

void KeyboardInputManager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_W) {
        // ���磺�� W ����ִ��ĳ������
    }
}

// MouseInputManager2 ʵ��
void MouseInputManager2::initialize() {
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(MouseInputManager2::onMouseScroll, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void MouseInputManager2::handleInput() {
    // �����������
}

void MouseInputManager2::onMouseScroll(cocos2d::Event* event) {
    // �����������¼������ڻ�������
}

// ButtonInputManager ʵ��
void ButtonInputManager::initialize() {
    // ��ʼ����ť����Ӽ�����
    auto button = cocos2d::ui::Button::create("button_normal.png", "button_pressed.png");
    button->setPosition(cocos2d::Vec2(100, 100));  // ���ð�ť��λ��
    button->addTouchEventListener(CC_CALLBACK_2(ButtonInputManager::onButtonClick, this));  // ��ӵ���¼�����
    cocos2d::Director::getInstance()->getRunningScene()->addChild(button);  // ��Ӱ�ť������
}

void ButtonInputManager::handleInput() {
    // ��ť�¼�ͨ���ص���������Ҫ���⴦��
}

void ButtonInputManager::onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
        CCLOG("Button clicked!");  // �����ť�����־
    }
}
