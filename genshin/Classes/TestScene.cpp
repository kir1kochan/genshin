#include "TestScene.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h"
#include "Classes/Common/EventManager/MainGameMouseEventManager.h"
#include "Classes/Common/Entities/Player/Player.h"
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
    // loadBackgroundMap();

    // 获取导演（Director）实例
    auto director = Director::getInstance();

    // 设置 OpenGL 的帧大小，可以增大这个尺寸来放大视图
    director->getOpenGLView()->setFrameSize(1920, 1080);  // 例如，使用更大的分辨率

    // 获取屏幕的可见大小
    Size visibleSize = director->getVisibleSize();

    // 设置设计分辨率大小，使用 NO_BORDER 可以确保视窗按比例填满
    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);


    scheduleOnce([this](float dt) {
        if (!blockManager) {
            blockManager = new BlockManager(static_cast<std::string>("/maps/world.tmx"));
        }
        }, 0.1f, "init_bm_key");

    // 测试模块 1
    addTestModule1();

    // 设置键盘事件监听器
    setupKeyboardListener();


    // 设置鼠标输入管理器用于视角缩放
    scheduleOnce([this](float dt) {
        if (!mouseInputManager) {
            mouseInputManager = new MainGameMouseEventManager;
            mouseInputManager->initialize();  // 初始化输入管理器
        }
        }, 0.1f, "init_mouse_manager_key");

    // 加入玩家
    scheduleOnce([this](float dt) {
        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        auto map = runningScene->getChildByName<cocos2d::TMXTiledMap*>("background");
        auto objectLayer = map->getObjectGroup("Objects");  // 获取对应图层
        auto spawnPoint = objectLayer->getObject("SpawnPoint");  // 获取对象点
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();
        auto playerspirt = Sprite::create("player.png");
        player = new Player(playerspirt);
        player->setPosition(x, y);

        this->addChild(player, 1);  // 将玩家加入到场景中
        player->setVisible(true);
        player->setScale(1.0f);
        cooking = new CookingSystem(player->getBackpack());
        this->addChild(cooking, 9);
        fishing = new FishingSystem;
        // 设置玩家输入管理器（例如键盘控制）
        if (!keyboardEventManager) {
            keyboardEventManager = new KeyboardEventManager;
            keyboardEventManager->initialize();
            keyboardEventManager->setPlayer(player);  // 将玩家对象传递给事件管理器
        }

        // 创建并设置背包主层
        backpackMainLayer = BackpackMainLayer::create(player);  // 使用 create 方法传入 player
        this->addChild(backpackMainLayer);  // 将 BackpackMainLayer 添加到场景中
        // 将背包层的 z 轴增高，确保它在前面
        backpackMainLayer->setLocalZOrder(10);
        // 初始化背包层时，放在屏幕外
        backpackMainLayer->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, 0));

        mouseInputManager->setPlayer(player);
        }, 0.1f, "init_player_key");

    scheduleOnce([this](float dt) {
        if (!spiritManager) {
            spiritManager = new SpiritManager();
            spiritManager->init(blockManager,player);
            spiritManager->setPlayer(player);
        }
        }, 0.1f, "init_SM_key");


    schedule([this](float deltaTime) {
        this->update(deltaTime);  // 每帧调用 update 方法
        }, 0.5f, "update_key");

    /*scheduleOnce([this](float dt) {
        if (!eq) {
            eq = EscortQuest::create(player,blockManager,spiritManager);
            this->addChild(eq);
        }
        }, 0.1f, "init_EQ_key");*/
    scheduleOnce([this](float dt) {
        if (!slSystem) {
            slSystem = new SLSystem();
            slSystem->setPlayer(player);
            slSystem->loadFromJson("/JSON/save1.json");
        }
        }, 0.1f, "init_EQ_key");
    return true;
}

void TestScene::loadBackgroundMap()
{
    // 加载 TMX 地图
    auto map = TMXTiledMap::create("/maps/forest.tmx");
    map->setName("background");
    this->addChild(map, -1);  // 将地图作为背景层添加到场景中
}

void TestScene::addTestModule1()
{
    // 如果模块已经添加过了，就不再添加
    if (this->getChildByName("module1")) {
        return;
    }
    if (player) {
        player->unequipWeapon();
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
    std::shared_ptr<Weapon> newWeapon = std::make_shared<Weapon>(100101, "Copper_Broadsword", 10, 30, 1.2);
    player->equipWeapon(newWeapon);
    player->levelUp();
    auto label = Label::createWithTTF("Test Module 2", "fonts/Marker Felt.ttf", 24);
    label->setName("module2");  // 给模块命名
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    player->addItemToBackpack(300302, 1);
    player->addItemToBackpack(300109, 1);
    auto slime = new Enemy();
    slime->setSpriteFilename("monsters/man_eater_flower.png");
    slime->generateSprite();
    slime->setPosition(5620, 985);
    this->addChild(label);
    this->addChild(slime);
    blockManager->addEnemy(slime);
    //player->testSkill();
    //player->setShield(150,5);
    //for (int i = 0; i < 30; i++) { player->levelUp(); }
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
        else if (keyCode == EventKeyboard::KeyCode::KEY_B) {
            CCLOG("Switching to Backpack");
            switchToBackpack();  // 切换到背包
            keyboardEventManager->setBackpackActive(true);
            mouseInputManager->setIsListening(false);
            is_running = false;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            CCLOG("Exiting Backpack");
            exitBackpack();  // 退出背包
            keyboardEventManager->setBackpackActive(false);
            mouseInputManager->setIsListening(true);
            is_running = true;
            // 创建并发送事件
            auto cookingEvent = new cocos2d::EventCustom("COOKING_ENDED_EVENT");
            _eventDispatcher->dispatchEvent(cookingEvent);  // 发送事件
            // 创建并发送事件
            auto fishingEvent = new cocos2d::EventCustom("FISHING_ENDED_EVENT");
            _eventDispatcher->dispatchEvent(fishingEvent);  // 发送事件
            auto mapEvent = new cocos2d::EventCustom("MAP_ENDED_EVENT");
            _eventDispatcher->dispatchEvent(mapEvent);  // 发送事件
            player->getChildByName("sprite")->setVisible(true);
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_F) {
            keyboardEventManager->setBackpackActive(true);
            mouseInputManager->setIsListening(false);
            // 创建并发送事件
            // auto cookingEvent = new cocos2d::EventCustom("COOKING_STARTED_EVENT");
            // _eventDispatcher->dispatchEvent(cookingEvent);  // 发送事件
            fishing = new FishingSystem();
            fishing->startFishing(this);
            fishing->setOnFishingResultCallback([this](bool success) {
                if (success) {
                    CCLOG("Fishing successful!");
                    player->addItemToBackpack(300101,1);
                }
                else {
                    CCLOG("Fishing failed!");
                }
                keyboardEventManager->setBackpackActive(false);
                mouseInputManager->setIsListening(true);
                scheduleOnce([this](float dt) {
                    this->removeChild(fishing);
                    delete fishing;
                    fishing = nullptr;
                    }, 2.0f, "delay_action_key"); 
                } );
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_M) {
            CCLOG("Toggling MiniMap");
            
            auto hud = player->getHud(); 
            if (hud->getIsExpanded()) {
                keyboardEventManager->setBackpackActive(false);
                mouseInputManager->setIsListening(true);
            }
            else {

                keyboardEventManager->setBackpackActive(true);
                mouseInputManager->setIsListening(false);
            }
            hud->toggleMiniMap();  // 切换小地图显示模式
        }
        };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
}

// 切换到背包
void TestScene::switchToBackpack()
{
    backpackMainLayer->setVisible(true);
    backpackMainLayer->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, 0));
    backpackMainLayer->adjustSizeForTransition();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Camera* camera = Camera::getDefaultCamera();
    Vec3 cameraPosition = camera->getPosition3D();
    Vec3 targetPosition = cameraPosition - Vec3(visibleSize.width / 2 - 275, visibleSize.height / 2 - 155, 380);
    // 背包层移动到目标位置
    auto moveTo = MoveTo::create(0.5f, targetPosition);  // 目标位置设置为 3D 坐标
    backpackMainLayer->runAction(moveTo);
    is_running = false;
    // 暂停玩家
    // player->setPaused(true);
}

// 退出背包
void TestScene::exitBackpack()
{
    auto moveOut = MoveTo::create(0.5f, Vec2(-Director::getInstance()->getVisibleSize().width, 0));
    auto spawn = Spawn::create(moveOut, nullptr);
    backpackMainLayer->runAction(Sequence::create(spawn, CallFunc::create([this]() {
        // 动画完成后，确保背包层完全不可见并隐藏
        backpackMainLayer->setVisible(false);
        }), nullptr));
    is_running = true;
    // 恢复玩家
    // player->setPaused(false);
}

TestScene::~TestScene()
{
    delete mouseInputManager;  // 释放鼠标输入管理器
    delete keyboardEventManager;  // 释放键盘事件管理器
    delete backpackMainLayer;  // 释放背包层
}

void TestScene::update(float deltaTime)
{
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(player->getPosition3D() + Vec3(0, 0, mouseInputManager->getCameraZ()));
    }
    if (keyboardEventManager) {
        keyboardEventManager->update(deltaTime);  // 调用键盘事件管理器的 update 方法
    }
    if (blockManager && is_running) {
        blockManager->updateBlocksForPlayer(player);
    }
    if (spiritManager && is_running) {
        if (gaptime < 0.5) {
            gaptime += deltaTime;
            return;
        }
        spiritManager->update(gaptime);
        nearestEnemy = spiritManager->getNearestEnemy();
        mouseInputManager->setNearestEnemy(nearestEnemy);
        keyboardEventManager->setNearestEnemy(nearestEnemy);
        gaptime = 0;
    }
    if (player&&is_running) {
        player->update(deltaTime);
    }
}

void TestScene::onExit() {
    Scene::onExit();
    // 这里可以进行数据保存
    slSystem->getPlayerPosition();
    slSystem->saveToJson("JSON/save1.json");
}

void TestScene::onEnter() {
    Scene::onEnter();
    // 恢复相机的状态
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(_savedCameraPosition);
    }
    // 这里可以继续数据读取
}


void TestScene::loadCameraPosition(){
    // 恢复相机的状态
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(player->getPosition3D() + Vec3(0, 0, mouseInputManager->getCameraZ()));
    }
}

