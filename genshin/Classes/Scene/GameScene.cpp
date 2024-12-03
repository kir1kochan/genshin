#include "GameScene.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    // 加载 TMX 地图作为背景
    loadBackgroundMap();

    // 获取导演（Director）实例
    auto director = Director::getInstance();

    // 设置 OpenGL 的帧大小
    director->getOpenGLView()->setFrameSize(1920, 1080);  // 例如，使用更大的分辨率

    // 获取屏幕的可见大小
    Size visibleSize = director->getVisibleSize();

    // 设置设计分辨率大小
    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);

    // 初始化游戏相关的管理器和玩家
    initGameManagers();

    // 初始化传送点
    loadTPAnchors();

    // 初始化输入管理器
    setupInputManager();

    return true;
}

void GameScene::initGameManagers()
{
    player = new Player();
    spiritManager = new SpiritManager();
    blockManager = new BlockManager();

    // 添加玩家到场景
    this->addChild(player);
    // 在此可以将 spiritManager 和 blockManager 等添加到场景或其他组件中
}

void GameScene::setupInputManager()
{
    inputManager = new InputManager();
    inputManager->setInputListener(this);  // 设置输入监听
}

void GameScene::loadBackgroundMap()
{
    auto map = TMXTiledMap::create("/maps/game_map.tmx");
    map->setName("background");
    this->addChild(map, -1);  // 将地图作为背景层添加到场景中
}

void GameScene::loadTPAnchors()
{
    tpAnchor = new TPAnchor();
    tpAnchor->loadFromJson("/maps/tp_points.json");  // 从文件加载传送点数据
    // 在场景中添加传送点的图像等
}

void GameScene::update(float deltaTime)
{
    // 更新游戏状态
    inputManager->update(deltaTime);
    player->update(deltaTime);
    // 可能还需要更新其它管理器的状态，例如 SpiritManager、BlockManager 等
}

void GameScene::onEnter()
{
    Scene::onEnter();
    // 初始化一些资源，或者绑定事件 未来应该会用于从tmx地图中读取并初始化碰撞箱相关内容
}

void GameScene::onExit()
{
    Scene::onExit();
    // 清理资源，例如释放管理器等
    delete player;
    delete inputManager;
    delete spiritManager;
    delete blockManager;
    delete tpAnchor;
}

GameScene::~GameScene()
{
    // 析构函数清理资源
    CCLOG("GameScene destroyed");
}
