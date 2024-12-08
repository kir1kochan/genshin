#include "TestScene.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h" 
#include "Classes/Common/EventManager/MainGameMouseEventManager.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "Classes/Tools/ToolFunction.h"
USING_NS_CC;

Scene* TestScene::createScene()
{
    return TestScene::create();
}

bool TestScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    // 加载 TMX 地图作为背景
    loadBackgroundMap();

    // 获取导演（Director）实例
    auto director = Director::getInstance();

    // 设置 OpenGL 的帧大小，可以增大这个尺寸来放大视图
    director->getOpenGLView()->setFrameSize(1920, 1080);  // 例如，使用更大的分辨率

    // 获取屏幕的可见大小
    Size visibleSize = director->getVisibleSize();

    // 设置设计分辨率大小，使用 NO_BORDER 可以确保视窗按比例填满
    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);

    // 如果地图太小，可以调整缩放比例，放大地图
    auto map = TMXTiledMap::create("/maps/test.tmx");
    if (map) {
        map->setName("background");
        this->addChild(map, -1);  // 将地图作为背景层添加到场景中
    }

    // 测试模块 1
    addTestModule1();

    // 设置键盘事件监听器
    setupKeyboardListener();

    // 延迟设置相机，确保场景已经初始化
    scheduleOnce([this](float dt) {
        auto director = Director::getInstance();
        Camera* camera = Camera::getDefaultCamera();
        camera->setPosition3D(Vec3(1400, 1400, 500));  // 改变摄像头的位置
        camera->lookAt(Vec3(1400, 1400, 0));  // 朝向场景中心
        }, 0, "init_camera_key");  // 延迟执行，相机设置将在场景初始化后执行

    // 设置鼠标输入管理器用于视角缩放
    scheduleOnce([this](float dt) {
        if (!mouseInputManager) {
            mouseInputManager = new MainGameMouseEventManager;
            mouseInputManager->initialize();  // 初始化输入管理器
        }
        }, 0.1f, "init_mouse_manager_key");

    // 加入玩家
    scheduleOnce([this,map](float dt) {
        auto objectLayer = map->getObjectGroup("object 1");  // 获取对应图层
        auto spawnPoint = objectLayer->getObject("SpawnPoint");  // 获取对象点
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();
        Vec2 playerInitialPosition = convertToSecenPoint(map, x, y);  // 转换获取当前点的世界坐标
        auto playerspirt = Sprite::create("player.png");
        auto player = new Player(playerspirt);
        player->setPosition(playerInitialPosition.x, playerInitialPosition.y);

        this->addChild(player, 1);  // 将玩家加入到场景中
        player->setVisible(true);
        player->setScale(1.0f);

        // 设置玩家输入管理器（例如键盘控制）
        if (!keyboardEventManager) {
            keyboardEventManager = new KeyboardEventManager;
            keyboardEventManager->initialize();
            keyboardEventManager->setPlayer(player);  // 将玩家对象传递给事件管理器
        }
        }, 0.1f, "init_player_key");

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // 每帧调用 update 方法
        }, 0.5f, "update_key");

    return true;
}

void TestScene::loadBackgroundMap()
{
    // 加载 TMX 地图
    auto map = TMXTiledMap::create("/maps/test.tmx");
    map->setName("background");
    this->addChild(map, -1);  // 将地图作为背景层添加到场景中
}

void TestScene::addTestModule1()
{
    // 如果模块已经添加过了，就不再添加
    if (this->getChildByName("module1")) {
        return;
    }
    auto label = Label::createWithTTF("Test Module 1", "fonts/Marker Felt.ttf", 24);
    label->setName("module1");  // 给模块命名
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(label);
}

void TestScene::addTestModule2()
{
    // 如果模块已经添加过了，就不再添加
    if (this->getChildByName("module2")) {
        return;
    }
    auto label = Label::createWithTTF("Test Module 2", "fonts/Marker Felt.ttf", 24);
    label->setName("module2");  // 给模块命名
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(label);
}

void TestScene::setupKeyboardListener()
{
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_1) {
            CCLOG("Switching to Test Module 1");
            this->removeChildByName("module2"); // 移除 Test Module 2
            addTestModule1();
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
            CCLOG("Switching to Test Module 2");
            this->removeChildByName("module1"); // 移除 Test Module 1
            addTestModule2();
        }
        };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

}

TestScene::~TestScene()
{
    delete mouseInputManager;  // 释放鼠标输入管理器
    delete keyboardEventManager;  // 释放键盘事件管理器
}

void TestScene::update(float deltaTime)
{
    if (keyboardEventManager) {
        keyboardEventManager->update(deltaTime);  // 调用键盘事件管理器的 update 方法
    }

}
