#ifndef __MAIN_GAME_MOUSE_EVENT_MANAGER_H__
#define __MAIN_GAME_MOUSE_EVENT_MANAGER_H__

#include "EventManager.h"

class MainGameMouseEventManager : public EventManager {
public:
    void initialize() override;
    void handleEvent() override;
    void onMouseDown(cocos2d::Event* event);
    void onMouseScroll(cocos2d::Event* event);
};

#endif // __MAIN_GAME_MOUSE_EVENT_MANAGER_H__
