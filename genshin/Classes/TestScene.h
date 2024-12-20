#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "cocos2d.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h" 
#include "Classes/Common/EventManager/MainGameMouseEventManager.h" 
#include "Classes/Scene/BackpackScene/BackpackMainLayer.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "Classes/Common/SpiritManager/SpiritManager.h"
#include "System/FishingSystem/FishingSystem.h"
#include "System/CookingSystem/CookingSystem.h"
class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual ~TestScene();  // �����������ͷ���Դ

    virtual bool init();

    // ��Ӳ���ģ��
    void addTestModule1();
    void addTestModule2();

    // �����¼�����
    void setupKeyboardListener();

    // ���� TMX ��ͼ����
    void loadBackgroundMap();

    void switchToBackpack();
    void exitBackpack();
    // ÿ֡����
    void update(float deltaTime);

    // �ָ����λ��
    void loadCameraPosition() ;

    CREATE_FUNC(TestScene);

private:
    MainGameMouseEventManager* mouseInputManager;  // ������������
    KeyboardEventManager* keyboardEventManager;  // �����¼�������
    Player* player;  // ��Ҷ���ָ��
    BackpackMainLayer* backpackMainLayer;  // ��������
    SpiritManager* spiritManager;
    BlockManager* blockManager;
    Enemy* nearestEnemy=nullptr;
    FishingSystem* fishing = nullptr;
    CookingSystem* cooking = nullptr;
    float gaptime=0;
    bool is_running = true;
    Vec3 _savedCameraPosition=Vec3(0,0,500);

    virtual void onExit() override;
    virtual void onEnter() override;
};

#endif // TESTSCENE_H
