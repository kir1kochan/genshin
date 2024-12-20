#ifndef __KEYBOARD_EVENT_MANAGER_H__
#define __KEYBOARD_EVENT_MANAGER_H__

#include "EventManager.h"
#include "Classes/Common/Entities/Player/Player.h"

class KeyboardEventManager : public EventManager {
public:
    ~KeyboardEventManager();
    void initialize() override;
    void handleEvent() override;
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void setPlayer(Player* player);
    void setNearestEnemy(Enemy* enemy);
    void update(float deltaTime, CCTMXTiledMap* map);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 设置背包界面的状态
    void setBackpackActive(bool active);

private:
    Player* player = nullptr;   // 存放玩家指针
    float moveSpeed = 30;        // 存放玩家速度
    cocos2d::Vec3 destination;  // 存放玩家位置
    bool isKeyPressed = false;  // 存放按钮是否按下的判断
    cocos2d::Vec2 moveDirection;
    bool isBackpackActive = false;  // 标记背包界面是否激活
    Enemy* nearestEnemy=nullptr;
    Enemy* defaultEnemy;
};

#endif // __KEYBOARD_EVENT_MANAGER_H__
