#ifndef __INVENTORY_MOUSE_EVENT_MANAGER_H__
#define __INVENTORY_MOUSE_EVENT_MANAGER_H__

#include "EventManager.h"

class InventoryMouseEventManager : public EventManager {
public:
    void initialize() override;
    void handleEvent() override;
    void onMouseScroll(cocos2d::Event* event);
};

#endif // __INVENTORY_MOUSE_EVENT_MANAGER_H__
