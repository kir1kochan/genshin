#include "BackpackLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

BackpackLayer::BackpackLayer() {}

BackpackLayer::~BackpackLayer() {}

bool BackpackLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    

    return true;
}



void BackpackLayer::setPlayer(Player* player) {
    this->player = player;
    if (player) {
        backpack = player->getBackpack();
        if (backpack) {
            auto newspriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Blue_Front1.png");
        }
    }
}

void BackpackLayer::createItemIcon() {
    int num;
    auto icon=Sprite::create()
}