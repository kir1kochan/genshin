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
    virtual ~TestScene();  // 析构函数，释放资源

    virtual bool init();

    // 添加测试模块
    void addTestModule1();
    void addTestModule2();

    // 键盘事件处理
    void setupKeyboardListener();

    // 加载 TMX 地图背景
    void loadBackgroundMap();

    void switchToBackpack();
    void exitBackpack();
    // 每帧更新
    void update(float deltaTime);

    CREATE_FUNC(TestScene);

private:
    MainGameMouseEventManager* mouseInputManager;  // 鼠标输入管理器
    KeyboardEventManager* keyboardEventManager;  // 键盘事件管理器
    Player* player;  // 玩家对象指针
    BackpackMainLayer* backpackMainLayer;  // 背包主层
};

#endif // TESTSCENE_H
