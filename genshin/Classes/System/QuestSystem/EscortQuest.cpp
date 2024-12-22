#include "EscortQuest.h"

// ��̬��������
EscortQuest* EscortQuest::create(Player* realPlayer,BlockManager* bm,SpiritManager* sm) {
    EscortQuest* quest = new (std::nothrow) EscortQuest();
    
    if (quest && quest->init(realPlayer,bm,sm)) {
        quest->autorelease();
        return quest;
    }
    CC_SAFE_DELETE(quest);
    return nullptr;
}

// ��ʼ������
bool EscortQuest::init(Player* realPlayer, BlockManager* bm, SpiritManager* sm) {
    if (!Node::init()) {
        return false;
    }
    // ����һ�� DrawNode ���ư�Χ��
    auto drawNode = cocos2d::DrawNode::create();
    drawNode->drawRect(startArea.origin,startArea.origin+startArea.size,
        cocos2d::Color4F::RED);
    this->escortTargetBM2 = bm;
    this->escortTargetSM2 = sm;
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
    gaptime += delta;
    if(gaptime>2.0){
        escortTargetSM2->update(gaptime);
        gaptime = 0;
    }
}

// �������Ƿ��������ʼ����
void EscortQuest::checkStartCondition() {
    if (startArea.containsPoint(realPlayer->getPosition())) {
        missionStarted = true;
        for (int i = 0; i < 5; i++) {
            auto enemy = new Enemy();
            enemy->setSpriteFilename("monsters/slime.png");
            enemy->generateSprite();
            enemy->setPosition(5620+20, 1585 + 200 * i);
            this->addChild(enemy);
            escortTargetBM1->addEnemy(enemy);
            escortTargetBM2->addEnemy(enemy);

        }
        // ���û���Ŀ���ƶ���Ŀ��λ��
        auto moveTo = cocos2d::MoveTo::create(90.0f, destination); // �ƶ�ʱ����Ե���
        escortTarget->runAction(moveTo);

        CCLOG("Escort Mission started!");
    }
}

// ����������״̬
void EscortQuest::checkMissionStatus() {
    if (escortTarget->getPosition().distance(destination) < 1.0f&&realPlayer->getPosition().distance(destination) < 50.0f) {
        CCLOG("Escort Mission completed successfully!");
        unscheduleUpdate(); // ֹͣ����
        return;
    }

    if (realPlayer->getHealth() <= 0) {
        CCLOG("Escort Mission failed! The escort target has died.");
        unscheduleUpdate(); // ֹͣ����
        return;
    }
}
