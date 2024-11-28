#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "cocos2d.h"
#include "Classes/Common/InputManager/InputManager.h" 

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

    CREATE_FUNC(TestScene);

private:
    MouseInputManager1* mouseInputManager;  // ������������
};
#endif // TESTSCENE_H
