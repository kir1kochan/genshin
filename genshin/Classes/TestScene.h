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

    // 恢复相机位置
    void loadCameraPosition() ;

    CREATE_FUNC(TestScene);

private:
    MainGameMouseEventManager* mouseInputManager;  // 鼠标输入管理器
    KeyboardEventManager* keyboardEventManager;  // 键盘事件管理器
    Player* player;  // 玩家对象指针
    BackpackMainLayer* backpackMainLayer;  // 背包主层
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
