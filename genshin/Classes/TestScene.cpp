#include "TestScene.h"
#include "Classes/Common/InputManager/InputManager.h" 
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
    auto map = this->getChildByName("background");  // 假设地图已经作为背景添加
    if (map) {
        map->setScale(1.5f);  // 设置适当的缩放比例
    }

    // 测试模块 1
    addTestModule1();


    // 设置键盘事件监听器
    setupKeyboardListener();

    // 延迟设置相机，确保场景已经初始化
    scheduleOnce([this](float dt) {
        auto director = Director::getInstance();
        Camera* camera = Camera::getDefaultCamera();
        camera->setPosition3D(Vec3(1280, 460, 500));  // 改变摄像头的位置
        camera->lookAt(Vec3(1280, 460, 0));  // 朝向场景中心

        }, 0, "init_camera_key");  // 延迟执行，相机设置将在场景初始化后执行

    // 设置鼠标输入管理器用于视角缩放
    scheduleOnce([this](float dt) {
        mouseInputManager = new MouseInputManager1();
        mouseInputManager->initialize();  // 在场景初始化后才初始化输入管理器
        }, 0.1f, "init_mouse_manager_key");

    return true;
}

void TestScene::loadBackgroundMap()
{
    // 加载 TMX 地图
    auto map = TMXTiledMap::create("test_map.tmx");
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
}