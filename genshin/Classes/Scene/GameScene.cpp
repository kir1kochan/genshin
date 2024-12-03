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

    // ���� TMX ��ͼ��Ϊ����
    loadBackgroundMap();

    // ��ȡ���ݣ�Director��ʵ��
    auto director = Director::getInstance();

    // ���� OpenGL ��֡��С
    director->getOpenGLView()->setFrameSize(1920, 1080);  // ���磬ʹ�ø���ķֱ���

    // ��ȡ��Ļ�Ŀɼ���С
    Size visibleSize = director->getVisibleSize();

    // ������Ʒֱ��ʴ�С
    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);

    // ��ʼ����Ϸ��صĹ����������
    initGameManagers();

    // ��ʼ�����͵�
    loadTPAnchors();

    // ��ʼ�����������
    setupInputManager();

    return true;
}

void GameScene::initGameManagers()
{
    player = new Player();
    spiritManager = new SpiritManager();
    blockManager = new BlockManager();

    // �����ҵ�����
    this->addChild(player);
    // �ڴ˿��Խ� spiritManager �� blockManager ����ӵ����������������
}

void GameScene::setupInputManager()
{
    inputManager = new InputManager();
    inputManager->setInputListener(this);  // �����������
}

void GameScene::loadBackgroundMap()
{
    auto map = TMXTiledMap::create("/maps/game_map.tmx");
    map->setName("background");
    this->addChild(map, -1);  // ����ͼ��Ϊ��������ӵ�������
}

void GameScene::loadTPAnchors()
{
    tpAnchor = new TPAnchor();
    tpAnchor->loadFromJson("/maps/tp_points.json");  // ���ļ����ش��͵�����
    // �ڳ�������Ӵ��͵��ͼ���
}

void GameScene::update(float deltaTime)
{
    // ������Ϸ״̬
    inputManager->update(deltaTime);
    player->update(deltaTime);
    // ���ܻ���Ҫ����������������״̬������ SpiritManager��BlockManager ��
}

void GameScene::onEnter()
{
    Scene::onEnter();
    // ��ʼ��һЩ��Դ�����߰��¼� δ��Ӧ�û����ڴ�tmx��ͼ�ж�ȡ����ʼ����ײ���������
}

void GameScene::onExit()
{
    Scene::onExit();
    // ������Դ�������ͷŹ�������
    delete player;
    delete inputManager;
    delete spiritManager;
    delete blockManager;
    delete tpAnchor;
}

GameScene::~GameScene()
{
    // ��������������Դ
    CCLOG("GameScene destroyed");
}
