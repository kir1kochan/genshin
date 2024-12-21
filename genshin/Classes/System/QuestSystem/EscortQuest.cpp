#include "EscortQuest.h"

// ��̬��������
EscortQuest* EscortQuest::create(Player* realPlayer) {
    EscortQuest* quest = new (std::nothrow) EscortQuest();
    if (quest && quest->init(realPlayer)) {
        quest->autorelease();
        return quest;
    }
    CC_SAFE_DELETE(quest);
    return nullptr;
}

// ��ʼ������
bool EscortQuest::init(Player* realPlayer) {
    if (!Node::init()) {
        return false;
    }
    // ����һ�� DrawNode ���ư�Χ��
    auto drawNode = cocos2d::DrawNode::create();
    drawNode->drawRect(startArea.origin,startArea.origin+startArea.size,
        cocos2d::Color4F::RED);

    // ��ӵ����ڵ�
    this->addChild(drawNode);
    auto sprite = cocos2d::Sprite::create("player.png");
    escortTarget =new Player(1000, sprite);
    this->realPlayer = realPlayer;
    this->startArea = startArea;
    this->destination = destination;
    this->missionStarted = false;

    // �����Ͷ������Ϊ�ӽڵ�
    this->addChild(escortTarget);

    // ���ó�ʼλ��
    escortTarget->setPosition(startArea.getMidX(), startArea.getMidY());

    // ���� update ����
    scheduleUpdate();

    return true;
}

// ÿ֡����
void EscortQuest::update(float delta) {
    if (!missionStarted) {
        checkStartCondition();
    }
    else {
        checkMissionStatus();
    }
}

// �������Ƿ��������ʼ����
void EscortQuest::checkStartCondition() {
    if (startArea.containsPoint(realPlayer->getPosition())) {
        missionStarted = true;

        // ���û���Ŀ���ƶ���Ŀ��λ��
        auto moveTo = cocos2d::MoveTo::create(10.0f, destination); // �ƶ�ʱ����Ե���
        escortTarget->runAction(moveTo);

        CCLOG("Escort Mission started!");
    }
}

// ����������״̬
void EscortQuest::checkMissionStatus() {
    if (escortTarget->getPosition().distance(destination) < 1.0f) {
        CCLOG("Escort Mission completed successfully!");
        unscheduleUpdate(); // ֹͣ����
        return;
    }

    if (escortTarget->getHealth() <= 0) {
        CCLOG("Escort Mission failed! The escort target has died.");
        unscheduleUpdate(); // ֹͣ����
        return;
    }
}
