#include "TestScene.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h"
#include "Classes/Common/EventManager/MainGameMouseEventManager.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "Classes/Common/Entities/NPC.h"
#include "Classes/Common/StoryManager/StoryManager.h"
USING_NS_CC;

Scene* TestScene::createScene()
{
    return TestScene::create();
}
void TestScene::checkPlayerAndNpcDistance() {
   float distance = player->getPosition().distance(npc->getPosition());  // 计算玩家和 NPC 之间的距离
   // 如果距离小于 100，触发对话
   if (distance < 100.0f) {
       npc->startDialogue();
   }
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
            blockManager = new BlockManager(static_cast<std::string>("maps/world.tmx"));
        }
        }, 0.1f, "init_bm_key");


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
        auto playerspirt = Sprite::create("player.png");
        player = new Player(playerspirt);

        this->addChild(player, 1);  // 将玩家加入到场景中
        player->setVisible(true);
        player->setScale(1.0f);
        cooking = new CookingSystem(player->getBackpack());
        this->addChild(cooking, 9);
        fishing = new FishingSystem;
        if (!slSystem) {
            tpAnchor = new TPAnchor();
            slSystem = new SLSystem(tpAnchor);
            slSystem->setPlayer(player);
            tpAnchor->setPlayer(player);
            slSystem->loadFromJson("save1.json");
            tpAnchor->loadFromJson("JSON/TPAnchors.json");
            auto tpAnchors = tpAnchor->gettpPointActivation();
            for (auto& anchor : tpAnchors) {
                Vec2 pos = anchor.first;
                auto sprite = cocos2d::Sprite::create("Icon/Anchor.jpg");
                this->addChild(sprite);
                sprite->setName("Anchor");
                sprite->setPosition(pos.x, pos.y);

            }
        }
        player->getHud()->setTPAnchor(tpAnchor);
        tpAnchor->activateTPPoint(Vec2(1808.0, 6280.0));
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

    // 添加 NPC
    scheduleOnce([this](float dt) {
        // 获取 NPC 的位置（可以从地图中获取，或者手动设置）
        auto npc = NPC::create("/images/princess.png");  // 假设 NPC 使用 "npc.png" 图片
        npc->setPosition(Vec2(5815, 345));  // 设置 NPC 的位置
        this->addChild(npc, 2);  // 将 NPC 加入场景中

        npc->startDialogue();
        }, 0.1f, "init_npc_key");

    scheduleOnce([this](float dt) {
        // 创建剧情管理器并启动剧情
        if (!storyManager) {
            storyManager = StoryManager::create();
            this->addChild(storyManager, 3);  // 将剧情管理器添加到场景
        }

        // 启动一段剧情
        storyManager->startStory("Welcome to the world! This is your adventure!\nLevel up to 100 throught tough battle and you will complete this game");
        }, 0.1f, "init_story_key");

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // 每帧调用 update 方法
        }, 0.5f, "update_key");

    /*scheduleOnce([this](float dt) {
        if (!eq) {
            eq = EscortQuest::create(player,blockManager,spiritManager);
            this->addChild(eq);
        }
        }, 0.1f, "init_EQ_key");*/



    auto listener1 = cocos2d::EventListenerCustom::create("COOKING_STARTED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Cooking event received!");
        keyboardEventManager->setBackpackActive(true);
        mouseInputManager->setIsListening(false);
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);  // 将监听器添加到事件分发器

    auto listener2 = cocos2d::EventListenerCustom::create("FISHING_STARTED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Fishing event received!");
        keyboardEventManager->setBackpackActive(true);
        mouseInputManager->setIsListening(false);
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);  // 将监听器添加到事件分发器

    return true;
}

void TestScene::loadBackgroundMap()
{
    // 加载 TMX 地图
    auto map = TMXTiledMap::create("/maps/forest.tmx");
    map->setName("background");
    this->addChild(map, -1);  // 将地图作为背景层添加到场景中
}


void TestScene::setupKeyboardListener()
{
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_B) {
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
            blockManager->handleClickEvent(player->getPosition(),player);
            /*keyboardEventManager->setBackpackActive(true);
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
                } );*/
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
        else if (keyCode == EventKeyboard::KeyCode::KEY_E) {
            // 假设 E 键触发与 NPC 对话
            if (npc) {
                npc->startDialogue();
                if (storyManager) {
                    storyManager->startStory("This is a test story. Welcome!");
                }
            }
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

bool TestScene::isPlayerNearNPC(Player* player, NPC* npc) {
    // 计算玩家和 NPC 之间的距离
    float distance = player->getPosition().distance(npc->getPosition());
    return distance < 100.0f;  // 如果小于 100 像素，则认为玩家靠近 NPC
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
    if (npc && player) {
        // 检查玩家是否靠近 NPC
        if (isPlayerNearNPC(player, npc)) {
            npc->startDialogue();  // 启动 NPC 对话
        }
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

