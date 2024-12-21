#include "EscortQuest.h"

// 静态创建方法
EscortQuest* EscortQuest::create(Player* realPlayer) {
    EscortQuest* quest = new (std::nothrow) EscortQuest();
    if (quest && quest->init(realPlayer)) {
        quest->autorelease();
        return quest;
    }
    CC_SAFE_DELETE(quest);
    return nullptr;
}

// 初始化方法
bool EscortQuest::init(Player* realPlayer) {
    if (!Node::init()) {
        return false;
    }
    // 创建一个 DrawNode 绘制包围盒
    auto drawNode = cocos2d::DrawNode::create();
    drawNode->drawRect(startArea.origin,startArea.origin+startArea.size,
        cocos2d::Color4F::RED);

    // 添加到父节点
    this->addChild(drawNode);
    auto sprite = cocos2d::Sprite::create("player.png");
    escortTarget =new Player(1000, sprite);
    this->realPlayer = realPlayer;
    this->startArea = startArea;
    this->destination = destination;
    this->missionStarted = false;

    // 将护送对象添加为子节点
    this->addChild(escortTarget);

    // 设置初始位置
    escortTarget->setPosition(startArea.getMidX(), startArea.getMidY());

    // 开启 update 方法
    scheduleUpdate();

    return true;
}

// 每帧更新
void EscortQuest::update(float delta) {
    if (!missionStarted) {
        checkStartCondition();
    }
    else {
        checkMissionStatus();
    }
}

// 检查玩家是否进入任务开始区域
void EscortQuest::checkStartCondition() {
    if (startArea.containsPoint(realPlayer->getPosition())) {
        missionStarted = true;

        // 设置护送目标移动到目标位置
        auto moveTo = cocos2d::MoveTo::create(10.0f, destination); // 移动时间可以调整
        escortTarget->runAction(moveTo);

        CCLOG("Escort Mission started!");
    }
}

// 检查任务完成状态
void EscortQuest::checkMissionStatus() {
    if (escortTarget->getPosition().distance(destination) < 1.0f) {
        CCLOG("Escort Mission completed successfully!");
        unscheduleUpdate(); // 停止更新
        return;
    }

    if (escortTarget->getHealth() <= 0) {
        CCLOG("Escort Mission failed! The escort target has died.");
        unscheduleUpdate(); // 停止更新
        return;
    }
}
