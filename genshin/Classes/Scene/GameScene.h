#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "Player.h"
#include "InputManager.h"
#include "SpiritManager.h"
#include "BlockManager.h"
#include "TPAnchor.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    ~GameScene();  // 析构函数，释放资源

    bool init();

    // 更新游戏状态
    void update(float deltaTime);

    // 加载地图
    void loadBackgroundMap();

    // 初始化游戏管理器
    void initGameManagers();

    // 初始化输入管理器
    void setupInputManager();

    // 加载 TPAnchors
    void loadTPAnchors();

    CREATE_FUNC(GameScene);

protected:
    // 游戏管理器和玩家
    Player* player;               // 玩家类指针
    InputManager* inputManager;    // 输入管理类
    SpiritManager* spiritManager;  // 精灵管理器
    BlockManager* blockManager;    // 区块管理器
    TPAnchor* tpAnchor;            // 传送点类指针
};

#endif // GAMESCENE_H
