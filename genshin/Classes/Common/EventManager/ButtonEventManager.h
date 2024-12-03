#ifndef __BUTTON_EVENT_MANAGER_H__
#define __BUTTON_EVENT_MANAGER_H__

#include "EventManager.h"

class ButtonEventManager : public EventManager {
public:
    void initialize() override;
    void handleEvent() override;
    void onButtonClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // __BUTTON_EVENT_MANAGER_H__
