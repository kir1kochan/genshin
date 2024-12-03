#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

// 父类: EventManager
class EventManager {
public:
    virtual void initialize() = 0;   // 初始化事件监听器
    virtual void handleEvent() = 0;   // 处理事件
    virtual ~EventManager() {}        // 虚析构函数
};

#endif // __EVENT_MANAGER_H__
