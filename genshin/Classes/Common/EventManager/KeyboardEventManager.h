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

    // ���ñ��������״̬
    void setBackpackActive(bool active);

private:
    Player* player = nullptr;   // ������ָ��
    float moveSpeed = 30;        // �������ٶ�
    cocos2d::Vec3 destination;  // ������λ��
    bool isKeyPressed = false;  // ��Ű�ť�Ƿ��µ��ж�
    cocos2d::Vec2 moveDirection;
    bool isBackpackActive = false;  // ��Ǳ��������Ƿ񼤻�
    Enemy* nearestEnemy=nullptr;
    Enemy* defaultEnemy;
};

#endif // __KEYBOARD_EVENT_MANAGER_H__
