#ifndef __KEYBOARD_EVENT_MANAGER_H__
#define __KEYBOARD_EVENT_MANAGER_H__

#include "EventManager.h"
#include "Classes/Common/Entities/Player/Player.h"

class KeyboardEventManager : public EventManager {
public:
    void initialize() override;
    void handleEvent() override;
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void setPlayer(Player* player);
    void update(float deltaTime);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
private:
    Player* player = nullptr;   // 存放玩家指针
    float moveSpeed = 10;        // 存放玩家速度
    cocos2d::Vec3 destination;  // 存放玩家位置
    bool isKeyPressed = false; ;// 存放按钮是否按下的判断
    cocos2d::Vec2 moveDirection;
};
#endif // __KEYBOARD_EVENT_MANAGER_H__
