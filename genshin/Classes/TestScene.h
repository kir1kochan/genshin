#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "cocos2d.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h" 
#include "Classes/Common/EventManager/MainGameMouseEventManager.h" 

class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual ~TestScene();  // �����������ͷ���Դ

    virtual bool init();

    // ��Ӳ�������
    void addTestModule1();
    void addTestModule2();

    // �����¼�����
    void setupKeyboardListener();

    // ���� TMX ��ͼ����
    void loadBackgroundMap();

    void update(float deltaTime);

    CREATE_FUNC(TestScene);

private:
    MainGameMouseEventManager* mouseInputManager;  // ������������
    KeyboardEventManager* keyboardEventManager;
    Player player;
};
#endif // TESTSCENE_H
