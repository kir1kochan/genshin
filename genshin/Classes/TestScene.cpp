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
    float distance = player->getPosition().distance(npc->getPosition());  // ������Һ� NPC ֮��ľ���
    // �������С�� 100�������Ի�
    if (distance < 100.0f) {
        npc->startDialogue();
    }
}

bool TestScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    // ���� TMX ��ͼ��Ϊ����
    // loadBackgroundMap();

    // ��ȡ���ݣ�Director��ʵ��
    auto director = Director::getInstance();

    // ���� OpenGL ��֡��С��������������ߴ����Ŵ���ͼ
    director->getOpenGLView()->setFrameSize(1920, 1080);  // ���磬ʹ�ø���ķֱ���

    // ��ȡ��Ļ�Ŀɼ���С
    Size visibleSize = director->getVisibleSize();

    // ������Ʒֱ��ʴ�С��ʹ�� NO_BORDER ����ȷ���Ӵ�����������
    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);


    scheduleOnce([this](float dt) {
        if (!blockManager) {
            blockManager = new BlockManager(static_cast<std::string>("/maps/world.tmx"));
        }
        }, 0.1f, "init_bm_key");

    // ���ü����¼�������
    setupKeyboardListener();


    // ���������������������ӽ�����
    scheduleOnce([this](float dt) {
        if (!mouseInputManager) {
            mouseInputManager = new MainGameMouseEventManager;
            mouseInputManager->initialize();  // ��ʼ�����������
        }
        }, 0.1f, "init_mouse_manager_key");

    // �������
    scheduleOnce([this](float dt) {
        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        auto map = runningScene->getChildByName<cocos2d::TMXTiledMap*>("background");
        auto playerspirt = Sprite::create("player.png");
        player = new Player(playerspirt);

        this->addChild(player, 1);  // ����Ҽ��뵽������
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
        // ������������������������̿��ƣ�
        if (!keyboardEventManager) {
            keyboardEventManager = new KeyboardEventManager;
            keyboardEventManager->initialize();
            keyboardEventManager->setPlayer(player);  // ����Ҷ��󴫵ݸ��¼�������
        }

        // ���������ñ�������
        backpackMainLayer = BackpackMainLayer::create(player);  // ʹ�� create �������� player
        this->addChild(backpackMainLayer);  // �� BackpackMainLayer ��ӵ�������
        // ��������� z �����ߣ�ȷ������ǰ��
        backpackMainLayer->setLocalZOrder(10);
        // ��ʼ��������ʱ��������Ļ��
        backpackMainLayer->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, 0));

        mouseInputManager->setPlayer(player);
        }, 0.1f, "init_player_key");

    scheduleOnce([this](float dt) {
        if (!spiritManager) {
            spiritManager = new SpiritManager();
            spiritManager->init(blockManager, player);
            spiritManager->setPlayer(player);
        }
        }, 0.1f, "init_SM_key");

    // ��� NPC
    scheduleOnce([this](float dt) {
        // ��ȡ NPC ��λ�ã����Դӵ�ͼ�л�ȡ�������ֶ����ã�
        auto npc = NPC::create("/images/princess.png");  // ���� NPC ʹ�� "npc.png" ͼƬ
        npc->setPosition(Vec2(5815, 345));  // ���� NPC ��λ��
        this->addChild(npc, 2);  // �� NPC ���볡����

        npc->startDialogue();
        }, 0.1f, "init_npc_key");

    scheduleOnce([this](float dt) {
        // �����������������������
        if (!storyManager) {
            storyManager = StoryManager::create();
            this->addChild(storyManager, 3);  // �������������ӵ�����
        }

        // ����һ�ξ���
        storyManager->startStory("Welcome to the world! This is your adventure!\nLevel up to 100 throught tough battle and you will complete this game");
        }, 0.1f, "init_story_key");

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // ÿ֡���� update ����
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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);  // ����������ӵ��¼��ַ���

    auto listener2 = cocos2d::EventListenerCustom::create("FISHING_STARTED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Fishing event received!");
        keyboardEventManager->setBackpackActive(true);
        mouseInputManager->setIsListening(false);
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);  // ����������ӵ��¼��ַ���

    return true;
}

void TestScene::loadBackgroundMap()
{
    // ���� TMX ��ͼ
    auto map = TMXTiledMap::create("/maps/forest.tmx");
    map->setName("background");
    this->addChild(map, -1);  // ����ͼ��Ϊ��������ӵ�������
}

void TestScene::addTestModule1()
{
    // ���ģ���Ѿ���ӹ��ˣ��Ͳ������
    if (this->getChildByName("module1")) {
        return;
    }
    if (player) {
        player->unequipWeapon();
    }
    auto label = Label::createWithTTF("Test Module 1", "fonts/Marker Felt.ttf", 24);
    label->setName("module1");  // ��ģ������
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(label);
}

void TestScene::setupKeyboardListener()
{
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_1) {
            CCLOG("Switching to Test Module 1");
            this->removeChildByName("module2"); // �Ƴ� Test Module 2
            addTestModule1();
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
            CCLOG("Switching to Test Module 2");
            this->removeChildByName("module1"); // �Ƴ� Test Module 1
            addTestModule2();
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_B) {
            CCLOG("Switching to Backpack");
            switchToBackpack();  // �л�������
            keyboardEventManager->setBackpackActive(true);
            mouseInputManager->setIsListening(false);
            is_running = false;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            CCLOG("Exiting Backpack");
            exitBackpack();  // �˳�����
            keyboardEventManager->setBackpackActive(false);
            mouseInputManager->setIsListening(true);
            is_running = true;
            // �����������¼�
            auto cookingEvent = new cocos2d::EventCustom("COOKING_ENDED_EVENT");
            _eventDispatcher->dispatchEvent(cookingEvent);  // �����¼�
            // �����������¼�
            auto fishingEvent = new cocos2d::EventCustom("FISHING_ENDED_EVENT");
            _eventDispatcher->dispatchEvent(fishingEvent);  // �����¼�
            auto mapEvent = new cocos2d::EventCustom("MAP_ENDED_EVENT");
            _eventDispatcher->dispatchEvent(mapEvent);  // �����¼�
            player->getChildByName("sprite")->setVisible(true);
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_F) {
            blockManager->handleClickEvent(player->getPosition(), player);
            /*keyboardEventManager->setBackpackActive(true);
            mouseInputManager->setIsListening(false);
            // �����������¼�
            // auto cookingEvent = new cocos2d::EventCustom("COOKING_STARTED_EVENT");
            // _eventDispatcher->dispatchEvent(cookingEvent);  // �����¼�
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
            hud->toggleMiniMap();  // �л�С��ͼ��ʾģʽ
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_H) {
            int doorIndex = blockManager->checkPlayerInDoorArea(player->getPosition());
            if (doorIndex != -1) {
                switchMap(doorIndex);
            }
        }
        };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
}

// �л�������
void TestScene::switchToBackpack()
{
    backpackMainLayer->setVisible(true);
    backpackMainLayer->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, 0));
    backpackMainLayer->adjustSizeForTransition();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Camera* camera = Camera::getDefaultCamera();
    Vec3 cameraPosition = camera->getPosition3D();
    Vec3 targetPosition = cameraPosition - Vec3(visibleSize.width / 2 - 275, visibleSize.height / 2 - 155, 380);
    // �������ƶ���Ŀ��λ��
    auto moveTo = MoveTo::create(0.5f, targetPosition);  // Ŀ��λ������Ϊ 3D ����
    backpackMainLayer->runAction(moveTo);
    is_running = false;
    // ��ͣ���
    // player->setPaused(true);
}

// �˳�����
void TestScene::exitBackpack()
{
    auto moveOut = MoveTo::create(0.5f, Vec2(-Director::getInstance()->getVisibleSize().width, 0));
    auto spawn = Spawn::create(moveOut, nullptr);
    backpackMainLayer->runAction(Sequence::create(spawn, CallFunc::create([this]() {
        // ������ɺ�ȷ����������ȫ���ɼ�������
        backpackMainLayer->setVisible(false);
        }), nullptr));
    is_running = true;
    // �ָ����
    // player->setPaused(false);
}

TestScene::~TestScene()
{
    delete mouseInputManager;  // �ͷ�������������
    delete keyboardEventManager;  // �ͷż����¼�������
    delete backpackMainLayer;  // �ͷű�����
}

bool TestScene::isPlayerNearNPC(Player* player, NPC* npc) {
    // ������Һ� NPC ֮��ľ���
    float distance = player->getPosition().distance(npc->getPosition());
    return distance < 100.0f;  // ���С�� 100 ���أ�����Ϊ��ҿ��� NPC
}


void TestScene::update(float deltaTime)
{
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(player->getPosition3D() + Vec3(0, 0, mouseInputManager->getCameraZ()));
    }
    if (keyboardEventManager) {
        keyboardEventManager->update(deltaTime);  // ���ü����¼��������� update ����
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
    if (player && is_running) {
        player->update(deltaTime);
    }
    if (npc && player) {
        // �������Ƿ񿿽� NPC
        if (isPlayerNearNPC(player, npc)) {
            npc->startDialogue();  // ���� NPC �Ի�
        }
    }
}

void TestScene::onExit() {
    Scene::onExit();
    // ������Խ������ݱ���
    slSystem->getPlayerPosition();
    slSystem->saveToJson("JSON/save1.json");
}

void TestScene::onEnter() {
    Scene::onEnter();
    // �ָ������״̬
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(_savedCameraPosition);
    }
    // ������Լ������ݶ�ȡ
}


void TestScene::loadCameraPosition() {
    // �ָ������״̬
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(player->getPosition3D() + Vec3(0, 0, mouseInputManager->getCameraZ()));
    }
}

void TestScene::switchMap(int index) {
    // ��� index Ϊ 0����ʾ�����ڵ�ͼ�л��ش��ͼ
    if (index == 0) {
        // ��ʾ���ͼ
        auto oldMap = this->getChildByName("background");
        if (oldMap) {
            oldMap->setVisible(true);
        }

        // �Ƴ����ڵ�ͼ
        auto smallMap = this->getChildByName("smallMap");
        if (smallMap) {
            this->removeChild(smallMap);
        }

        // �ָ�����С��ͼ
        auto hud = player->getHud();
        if (hud) {
            hud->setMiniMapVisible(true);
            hud->setMiniMapPlayerPosition(playerPositionInMiniMap);
        }

        // ��������ûش��ͼ�е�λ��
        player->setPosition(playerPositionInWorld);
    }
    else {
        // ��¼����ڴ��ͼ�е�λ��
        playerPositionInWorld = player->getPosition();

        // ��������С��ͼ
        auto hud = player->getHud();
        if (hud) {
            playerPositionInMiniMap = hud->getMiniMapPlayerPosition();
            hud->setMiniMapVisible(false);
        }

        // �����µ� TMX ��ͼ
        std::string newMapFile = StringUtils::format("%d.tmx", index);
        auto newMap = TMXTiledMap::create(newMapFile);
        newMap->setName("smallMap");
        this->addChild(newMap, -1);

        // ����ԭ�ȵĴ��ͼ
        auto oldMap = this->getChildByName("background");
        if (oldMap) {
            oldMap->setVisible(false);
        }

        // ��������õ���ͼ�е� SpawnPoint λ��
        auto objectLayer = newMap->getObjectGroup("ObjectsLayer");
        auto spawnPoint = objectLayer->getObject("SpawnPoint");
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();
        player->setPosition(x, y);
    }
}