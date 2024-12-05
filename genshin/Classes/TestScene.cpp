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
    auto map = TMXTiledMap::create("/maps/test.tmx");
    if (map) {
        map->setName("background");
        this->addChild(map, -1);  // ����ͼ��Ϊ��������ӵ�������
    }

    // ����ģ�� 1
    addTestModule1();

    // ���ü����¼�������
    setupKeyboardListener();

    // �ӳ����������ȷ�������Ѿ���ʼ��
    scheduleOnce([this](float dt) {
        auto director = Director::getInstance();
        Camera* camera = Camera::getDefaultCamera();
        camera->setPosition3D(Vec3(1400, 1400, 500));  // �ı�����ͷ��λ��
        camera->lookAt(Vec3(1400, 1400, 0));  // ���򳡾�����
        }, 0, "init_camera_key");  // �ӳ�ִ�У�������ý��ڳ�����ʼ����ִ��

    // ���������������������ӽ�����
    scheduleOnce([this](float dt) {
        if (!mouseInputManager) {
            mouseInputManager = new MainGameMouseEventManager;
            mouseInputManager->initialize();  // ��ʼ�����������
        }
        }, 0.1f, "init_mouse_manager_key");

    // �������
    scheduleOnce([this,map](float dt) {
        auto objectLayer = map->getObjectGroup("object 1");  // ��ȡ��Ӧͼ��
        auto spawnPoint = objectLayer->getObject("SpawnPoint");  // ��ȡ�����
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();
        Vec2 playerInitialPosition = convertToSecenPoint(map, x, y);  // ת����ȡ��ǰ�����������
        auto playerspirt = Sprite::create("player.png");
        auto player = new Player(playerspirt);
        player->setPosition(playerInitialPosition.x, playerInitialPosition.y);

        this->addChild(player, 1);  // ����Ҽ��뵽������
        player->setVisible(true);
        player->setScale(1.0f);

        // ������������������������̿��ƣ�
        if (!keyboardEventManager) {
            keyboardEventManager = new KeyboardEventManager;
            keyboardEventManager->initialize();
            keyboardEventManager->setPlayer(player);  // ����Ҷ��󴫵ݸ��¼�������
        }
        }, 0.1f, "init_player_key");

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // ÿ֡���� update ����
        }, 0.5f, "update_key");

    return true;
}

void TestScene::loadBackgroundMap()
{
    // ���� TMX ��ͼ
    auto map = TMXTiledMap::create("/maps/test.tmx");
    map->setName("background");
    this->addChild(map, -1);  // ����ͼ��Ϊ��������ӵ�������
}

void TestScene::addTestModule1()
{
    // ���ģ���Ѿ���ӹ��ˣ��Ͳ������
    if (this->getChildByName("module1")) {
        return;
    }
    auto label = Label::createWithTTF("Test Module 1", "fonts/Marker Felt.ttf", 24);
    label->setName("module1");  // ��ģ������
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(label);
}

void TestScene::addTestModule2()
{
    // ���ģ���Ѿ���ӹ��ˣ��Ͳ������
    if (this->getChildByName("module2")) {
        return;
    }
    auto label = Label::createWithTTF("Test Module 2", "fonts/Marker Felt.ttf", 24);
    label->setName("module2");  // ��ģ������
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
        };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

}

TestScene::~TestScene()
{
    delete mouseInputManager;  // �ͷ�������������
    delete keyboardEventManager;  // �ͷż����¼�������
}

void TestScene::update(float deltaTime)
{
    if (keyboardEventManager) {
        keyboardEventManager->update(deltaTime);  // ���ü����¼��������� update ����
    }

}
