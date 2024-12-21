#ifndef ESCORT_QUEST_H
#define ESCORT_QUEST_H

#include "cocos2d.h"
#include "../../Common/Entities/Player/Player.h"
#include "../../Common/BlockManager/BlockManager.h"
#include "../../Common/SpiritManager/SpiritManager.h"
class EscortQuest : public cocos2d::Node {
public:
    // 静态创建方法
    static EscortQuest* create(Player* realPlayer);

    // 初始化方法
    bool init(Player* realPlayer);

    // 每帧更新
    void update(float delta) override;

private:
    // 成员变量
    Player* escortTarget;        // 被护送对象
    Player* realPlayer;          // 实际玩家
    cocos2d::Rect startArea=cocos2d::Rect(5620,1585,50,50);     // 任务开始区域
    cocos2d::Vec2 destination=cocos2d::Vec2(5620,2710);   // 目标位置
    bool missionStarted;         // 任务是否已经开始
    BlockManager* escortTargetBM;
    SpiritManager* escortTargetSM;
    // 检查玩家是否进入任务开始区域
    void checkStartCondition();

    // 检查任务完成状态
    void checkMissionStatus();
};

#endif // ESCORT_QUEST_H
