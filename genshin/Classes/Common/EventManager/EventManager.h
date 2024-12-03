#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

// ����: EventManager
class EventManager {
public:
    virtual void initialize() = 0;   // ��ʼ���¼�������
    virtual void handleEvent() = 0;   // �����¼�
    virtual ~EventManager() {}        // ����������
};

#endif // __EVENT_MANAGER_H__
