#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

// 父类: InputManager
class InputManager {
public:
    virtual void initialize() = 0;   // 初始化输入监听器
    virtual void handleInput() = 0;   // 处理输入事件
    virtual ~InputManager() {}        // 虚析构函数
};

// 子类: MouseInputManager1 (用于适配不同场景)
class MouseInputManager1 : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onMouseDown(cocos2d::Event* event);
    void onMouseScroll(cocos2d::Event* event);
};

// 子类: KeyboardInputManager (处理键盘输入)
class KeyboardInputManager : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

// 子类: MouseInputManager2 (另一个鼠标事件处理类，用于不同场景)
class MouseInputManager2 : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onMouseScroll(cocos2d::Event* event);
};

// 子类: ButtonInputManager (优先处理按钮的鼠标输入)
class ButtonInputManager : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // __INPUT_MANAGER_H__
