#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "cocos2d.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h" 
#include "Classes/Common/EventManager/MainGameMouseEventManager.h" 
#include "Classes/Scene/BackpackScene/BackpackMainLayer.h"
#include "Classes/Common/Entities/Player/Player.h"

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

    CREATE_FUNC(TestScene);

private:
    MainGameMouseEventManager* mouseInputManager;  // ������������
    KeyboardEventManager* keyboardEventManager;  // �����¼�������
    Player* player;  // ��Ҷ���ָ��
    BackpackMainLayer* backpackMainLayer;  // ��������
};

#endif // TESTSCENE_H
