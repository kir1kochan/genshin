/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT or OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

 // 该文件用于测试新增内容，原空项目的helloworldsense文件被保留了副本

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

// 添加 Spirit 相关头文件
#include "Common/Spirit.h"
#include "Common/Element.h"
#include "Common/Player/PlayerSpirit.h"
#include "Common/Enemy/EnemySpirit.h"

USING_NS_CC;


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

    /////////////////////////////
    // 2. 添加其他内容 ...

     // create and initialize a label
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(sprite, 0);
    }

    // 创建 Spirit 实例并测试
    Spirit* spirit = new Spirit(100.0f, Element::FIRE);  // 假设创建一个火属性的 Spirit
    CCLOG("Spirit Health: %.2f, Element: %s", spirit->getHealth(), elementToString(spirit->getElement()).c_str());

    // 创建 PlayerSpirit 实例并测试
    PlayerSpirit* playerSpirit = new PlayerSpirit(150.0f, Element::WATER);
    CCLOG("PlayerSpirit Health: %.2f, Element: %s, Name: %s", playerSpirit->getHealth(), elementToString(playerSpirit->getElement()).c_str(), playerSpirit->getName().c_str());

    // 创建 EnemySpirit 实例并测试
    EnemySpirit* enemySpirit = new EnemySpirit(80.0f, Element::EARTH, 10);
    CCLOG("EnemySpirit Health: %.2f, Element: %s, Attack: %d", enemySpirit->getHealth(), elementToString(enemySpirit->getElement()).c_str(), enemySpirit->getAttack());

    /////////////////////////////
    // 3. 添加 Spirit 信息的显示

    // 创建并显示 Spirit 的信息
    auto spiritLabel = Label::createWithTTF("Spirit Health: " + std::to_string(spirit->getHealth()) +
        ", Element: " + elementToString(spirit->getElement()),
        "fonts/Marker Felt.ttf", 24);
    spiritLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - spiritLabel->getContentSize().height - 40));
    this->addChild(spiritLabel, 1);

    // 创建并显示 PlayerSpirit 的信息
    auto playerLabel = Label::createWithTTF("PlayerSpirit Health: " + std::to_string(playerSpirit->getHealth()) +
        ", Element: " + elementToString(playerSpirit->getElement()) +
        ", Name: " + playerSpirit->getName(),
        "fonts/Marker Felt.ttf", 24);
    playerLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - playerLabel->getContentSize().height - 80));
    this->addChild(playerLabel, 1);

    // 创建并显示 EnemySpirit 的信息
    auto enemyLabel = Label::createWithTTF("EnemySpirit Health: " + std::to_string(enemySpirit->getHealth()) +
        ", Element: " + elementToString(enemySpirit->getElement()) +
        ", Attack: " + std::to_string(enemySpirit->getAttack()),
        "fonts/Marker Felt.ttf", 24);
    enemyLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - enemyLabel->getContentSize().height - 120));
    this->addChild(enemyLabel, 1);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}
