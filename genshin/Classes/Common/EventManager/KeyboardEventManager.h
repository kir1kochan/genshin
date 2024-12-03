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
    Player* player = nullptr;
    float moveSpeed = 10;
    cocos2d::Vec2 destination = nullptr;
};
#endif // __KEYBOARD_EVENT_MANAGER_H__
