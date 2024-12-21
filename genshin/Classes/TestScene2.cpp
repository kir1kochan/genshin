#include "TestScene.h"
#include "Classes/Common/EventManager/KeyboardEventManager.h"
#include "Classes/Common/EventManager/MainGameMouseEventManager.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "Classes/UI/Hud.h"
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

    // ���� TMX ��ͼ��Ϊ����
    loadBackgroundMap();

    // ��ȡ���ݣ�Director��ʵ��
    auto director = Director::getInstance();

    // ���� OpenGL ��֡��С��������������ߴ����Ŵ���ͼ
    director->getOpenGLView()->setFrameSize(1920, 1080);  // ���磬ʹ�ø���ķֱ���

    // ��ȡ��Ļ�Ŀɼ���С
    Size visibleSize = director->getVisibleSize();

    // ������Ʒֱ��ʴ�С��ʹ�� NO_BORDER ����ȷ���Ӵ�����������
    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);

    // �����ͼ̫С�����Ե������ű������Ŵ��ͼ
    auto map = TMXTiledMap::create("/maps/world2.tmx");
    if (map) {
        map->setName("background");
        this->addChild(map, -1);  // ����ͼ��Ϊ���������ӵ�������
    }

    auto layer = map->getLayer("area");
    layer->setVisible(false);

    hud = Hud::create(player);
    this->addChild(hud, 10);

    // ����ģ�� 1
    addTestModule1();

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
    scheduleOnce([this, map](float dt) {
        auto objectLayer = map->getObjectGroup("Objects");  // ��ȡ��Ӧͼ��
        auto spawnPoint = objectLayer->getObject("SpawnPoint");  // ��ȡ�����
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();
        auto playerspirt = Sprite::create("player.png");
        player = new Player(playerspirt);
        player->setPosition(x, y);

        this->addChild(player, 1);  // ����Ҽ��뵽������
        player->setVisible(true);
        player->setScale(1.0f);
        cooking = new CookingSystem(player->getBackpack());
        this->addChild(cooking, 9);
        fishing = new FishingSystem;
        // ������������������������̿��ƣ�
        if (!keyboardEventManager) {
            keyboardEventManager = new KeyboardEventManager;
            keyboardEventManager->initialize();
            keyboardEventManager->setPlayer(player);  // ����Ҷ��󴫵ݸ��¼�������
        }

        // ���������ñ�������
        backpackMainLayer = BackpackMainLayer::create(player);  // ʹ�� create �������� player
        this->addChild(backpackMainLayer);  // �� BackpackMainLayer ���ӵ�������
        // ��������� z �����ߣ�ȷ������ǰ��
        backpackMainLayer->setLocalZOrder(10);
        // ��ʼ��������ʱ��������Ļ��
        backpackMainLayer->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, 0));

        mouseInputManager->setPlayer(player);

        }, 0.1f, "init_player_key");

    scheduleOnce([this](float dt) {
        if (!blockManager) {
            blockManager=new BlockManager;
        }
        }, 0.1f, "init_bm_key");

    scheduleOnce([this](float dt) {
        if (!spiritManager) {
            spiritManager = new SpiritManager();
            spiritManager->init(blockManager,player);
            spiritManager->setPlayer(player);
        }
        }, 0.1f, "init_SM_key");


    schedule([this,map](float deltaTime) {
        this->update(deltaTime ,map);  // ÿ֡���� update ����
        }, 0.5f, "update_key");

    return true;
}

void TestScene::loadBackgroundMap()
{

}

void TestScene::addTestModule1()
{
    // ���ģ���Ѿ����ӹ��ˣ��Ͳ�������
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

void TestScene::addTestModule2()
{
    // ���ģ���Ѿ����ӹ��ˣ��Ͳ�������
    if (this->getChildByName("module2")) {
        return;
    }
    std::shared_ptr<Weapon> newWeapon = std::make_shared<Weapon>(100101, "Copper_Broadsword", 10, 30, 1.2);
    player->equipWeapon(newWeapon);
    player->levelUp();
    auto label = Label::createWithTTF("Test Module 2", "fonts/Marker Felt.ttf", 24);
    label->setName("module2");  // ��ģ������
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    player->addItemToBackpack(300302, 1);
    auto slime = new Enemy();
    slime->setSpriteFilename("monsters/slime.png");
    slime->generateSprite();
    slime->setPosition(1800, 6300);
    this->addChild(label);
    this->addChild(slime);
    blockManager->addEnemy(slime);
    //player->testSkill();
    //player->setShield(150,5);

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
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_F) {
            keyboardEventManager->setBackpackActive(true);
            mouseInputManager->setIsListening(false);
            // �����������¼�
            auto cookingEvent = new cocos2d::EventCustom("COOKING_STARTED_EVENT");
            _eventDispatcher->dispatchEvent(cookingEvent);  // �����¼�
            /* 
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
            hud->toggleMiniMap();  // �л�С��ͼ��ʾģʽ
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

void TestScene::update(float deltaTime, CCTMXTiledMap* map)
{
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(player->getPosition3D() + Vec3(0, 0, mouseInputManager->getCameraZ()));
    }
    if (keyboardEventManager) {
        keyboardEventManager->update(deltaTime,map);  // ���ü����¼��������� update ����
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
    // ������Խ������ݱ���
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


void TestScene::loadCameraPosition(){
    // �ָ������״̬
    auto camera = Camera::getDefaultCamera();
    if (camera) {
        camera->setPosition3D(player->getPosition3D() + Vec3(0, 0, mouseInputManager->getCameraZ()));
    }
}

//��ʾ��ɫ��С��ͼ�л�ʱ��
void TestScene::showMainMapAndPlayer() {
    auto map = this->getChildByName("background");
    if (map) {
        map->setVisible(true);
    }
    if (player) {
        player->setVisible(true);
    }
}

void TestScene::hideMainMapAndPlayer() {
    auto map = this->getChildByName("background");
    if (map) {
        map->setVisible(false);
    }
    if (player) {
        player->setVisible(false);
    }
}