#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

// ����: InputManager
class InputManager {
public:
    virtual void initialize() = 0;   // ��ʼ�����������
    virtual void handleInput() = 0;   // ���������¼�
    virtual ~InputManager() {}        // ����������
};

// ����: MouseInputManager1 (�������䲻ͬ����)
class MouseInputManager1 : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onMouseDown(cocos2d::Event* event);
    void onMouseScroll(cocos2d::Event* event);
};

// ����: KeyboardInputManager (�����������)
class KeyboardInputManager : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

// ����: MouseInputManager2 (��һ������¼������࣬���ڲ�ͬ����)
class MouseInputManager2 : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onMouseScroll(cocos2d::Event* event);
};

// ����: ButtonInputManager (���ȴ���ť���������)
class ButtonInputManager : public InputManager {
public:
    void initialize() override;
    void handleInput() override;
    void onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // __INPUT_MANAGER_H__
