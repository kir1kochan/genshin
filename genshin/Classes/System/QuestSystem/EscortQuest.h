#ifndef ESCORT_QUEST_H
#define ESCORT_QUEST_H

#include "cocos2d.h"
#include "../../Common/Entities/Player/Player.h"
#include "../../Common/BlockManager/BlockManager.h"
#include "../../Common/SpiritManager/SpiritManager.h"
class EscortQuest : public cocos2d::Node {
public:
    // ��̬��������
    static EscortQuest* create(Player* realPlayer);

    // ��ʼ������
    bool init(Player* realPlayer);

    // ÿ֡����
    void update(float delta) override;

private:
    // ��Ա����
    Player* escortTarget;        // �����Ͷ���
    Player* realPlayer;          // ʵ�����
    cocos2d::Rect startArea=cocos2d::Rect(5620,1585,50,50);     // ����ʼ����
    cocos2d::Vec2 destination=cocos2d::Vec2(5620,2710);   // Ŀ��λ��
    bool missionStarted;         // �����Ƿ��Ѿ���ʼ
    BlockManager* escortTargetBM;
    SpiritManager* escortTargetSM;
    // �������Ƿ��������ʼ����
    void checkStartCondition();

    // ����������״̬
    void checkMissionStatus();
};

#endif // ESCORT_QUEST_H
