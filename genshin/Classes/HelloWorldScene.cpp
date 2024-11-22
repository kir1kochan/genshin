#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

// 添加 Spirit 相关头文件
#include "Common/Spirit.h"        // 包含 Spirit 类
#include "Common/Player/PlayerSpirit.h"  // 包含 PlayerSpirit 类
#include "Common/Enemy/EnemySpirit.h"   // 包含 EnemySpirit 类

USING_NS_CC;


std::string elementToString(Element element) {
    switch (element) {
    case Element::FIRE: return "Fire";
    case Element::WATER: return "Water";
    case Element::EARTH: return "Earth";
    case Element::AIR: return "Air";
    case Element::LIGHT: return "Light";
    case Element::DARK: return "Dark";
    default: return "Unknown";
    }
}


Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Director::getInstance()->setClearColor(Color4F(0, 0, 0, 1)); // 设置黑色背景

    /////////////////////////////
    // 3. 创建一个标签显示
    auto label = Label::createWithSystemFont("Hello, Genshin!", "Arial", 24);
    label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(label);

    /////////////////////////////
    // 2. 添加其他内容 ...

    // 创建 Spirit 实例并测试

    Spirit* spirit = new Spirit();
    CCLOG("Spirit Health: %d, Element: %s", spirit->getHealth(), elementToString(spirit->getElement()).c_str());

    // 创建 PlayerSpirit 实例并测试
    PlayerSpirit* playerSpirit = new PlayerSpirit();
    CCLOG("PlayerSpirit Health: %d, Element: %s, Name: %s", playerSpirit->getHealth(), elementToString(playerSpirit->getElement()).c_str());

    // 创建 EnemySpirit 实例并测试
    EnemySpirit* enemySpirit = new EnemySpirit();
    CCLOG("EnemySpirit Health: %d, Element: %s", enemySpirit->getHealth(), elementToString(enemySpirit->getElement()).c_str());

    /////////////////////////////
    // 3. 继续添加其他代码

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}
