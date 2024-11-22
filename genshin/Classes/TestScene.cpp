#include "TestScene.h"

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

    // ²âÊÔÄ£¿é 1
    addTestModule1();

    // ÉèÖÃ¼üÅÌÊÂ¼þ¼àÌýÆ÷
    setupKeyboardListener();

    return true;
}

void TestScene::addTestModule1()
{
    auto label = Label::createWithTTF("Test Module 1", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(label);
}

void TestScene::addTestModule2()
{
    auto label = Label::createWithTTF("Test Module 2", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(label);
}

void TestScene::setupKeyboardListener()
{
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_1) {
            CCLOG("Switching to Test Module 1");
            this->removeAllChildren();
            addTestModule1();
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
            CCLOG("Switching to Test Module 2");
            this->removeAllChildren();
            addTestModule2();
        }
        };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
}
