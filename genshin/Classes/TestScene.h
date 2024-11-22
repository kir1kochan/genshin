#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "cocos2d.h"

class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // 添加测试内容
    void addTestModule1();
    void addTestModule2();

    // 键盘事件处理
    void setupKeyboardListener();

    CREATE_FUNC(TestScene);
};

#endif // TESTSCENE_H
