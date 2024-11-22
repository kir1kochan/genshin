#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "cocos2d.h"

class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // ��Ӳ�������
    void addTestModule1();
    void addTestModule2();

    // �����¼�����
    void setupKeyboardListener();

    CREATE_FUNC(TestScene);
};

#endif // TESTSCENE_H
