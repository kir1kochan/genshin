#include "InputManager.h"
#include "cocos2d.h"
USING_NS_CC;

// MouseInputManager1 实现
void MouseInputManager1::initialize() {
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(MouseInputManager1::onMouseScroll, this);
    listener->onMouseDown = CC_CALLBACK_1(MouseInputManager1::onMouseDown, this);  // 监听鼠标按下事件
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void MouseInputManager1::handleInput() {
    // 处理鼠标输入的逻辑
}

void MouseInputManager1::onMouseDown(cocos2d::Event* event) {
    // 处理鼠标按下事件
}

void MouseInputManager1::onMouseScroll(cocos2d::Event* event) {
    auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
    float deltaY = mouseEvent->getScrollY();
    auto director = cocos2d::Director::getInstance();
    Camera* camera = Camera::getDefaultCamera();

    // 增加或减少摄像机的Z坐标来模拟缩放
    Vec3 currentPos = camera->getPosition3D();
    if (deltaY > 0 && currentPos.z >= 400) {
        // 向前缩小
        currentPos.z -= 20;
    }
    else if (deltaY < 0 && currentPos.z <= 800){
        // 向后放大
        currentPos.z += 20;
    }
    camera->setPosition3D(currentPos);
}

// KeyboardInputManager 实现
void KeyboardInputManager::initialize() {
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(KeyboardInputManager::onKeyPressed, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void KeyboardInputManager::handleInput() {
    // 处理键盘输入
}

void KeyboardInputManager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_W) {
        // 例如：按 W 键，执行某个操作
    }
}

// MouseInputManager2 实现
void MouseInputManager2::initialize() {
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(MouseInputManager2::onMouseScroll, this);
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cocos2d::Director::getInstance()->getRunningScene());
}

void MouseInputManager2::handleInput() {
    // 处理鼠标输入
}

void MouseInputManager2::onMouseScroll(cocos2d::Event* event) {
    // 处理鼠标滚轮事件，用于滑动窗口
}

// ButtonInputManager 实现
void ButtonInputManager::initialize() {
    // 初始化按钮并添加监听器
    auto button = cocos2d::ui::Button::create("button_normal.png", "button_pressed.png");
    button->setPosition(cocos2d::Vec2(100, 100));  // 设置按钮的位置
    button->addTouchEventListener(CC_CALLBACK_2(ButtonInputManager::onButtonClick, this));  // 添加点击事件监听
    cocos2d::Director::getInstance()->getRunningScene()->addChild(button);  // 添加按钮到场景
}

void ButtonInputManager::handleInput() {
    // 按钮事件通过回调处理，不需要额外处理
}

void ButtonInputManager::onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
        CCLOG("Button clicked!");  // 输出按钮点击日志
    }
}
