#ifndef __MAIN_GAME_MOUSE_EVENT_MANAGER_H__
#define __MAIN_GAME_MOUSE_EVENT_MANAGER_H__

#include "EventManager.h"
#include "Classes/Common/Entities/Player/Player.h"

class MainGameMouseEventManager : public EventManager {
public:
    void initialize() override;
    void handleEvent() override;
    void onMouseDown(cocos2d::Event* event);
    void onMouseScroll(cocos2d::Event* event);
    void setPlayer(Player* player);
    void setNearestEnemy(Enemy* enemy);
    // �����Ƿ��������¼�
    void setIsListening(bool isListening) {
        _isListening = isListening;
    }

    float getCameraZ();

private:
    bool _isListening = true;  // Ĭ��Ϊ����
    Enemy* nearestEnemy = nullptr;
    Player* player = nullptr;   // ������ָ��
    Hud* hud = nullptr;
    float cameraZ=500.0f;
};

#endif // __MAIN_GAME_MOUSE_EVENT_MANAGER_H__
