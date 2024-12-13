#ifndef __QUEST_H__
#define __QUEST_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Backpack.h" // 假设 Backpack 类已经存在

struct QuestStage {
    std::unordered_map<int, int> requiredItems; // 每个阶段需要的物品及其数量 (itemId, quantity)
    cocos2d::Vec2 targetPosition;              // 每个阶段的目标位置
    std::string description;                   // 阶段任务描述
};

class Quest {
public:
    Quest(int id);
    ~Quest();

    // 获取任务 ID
    int getQuestId() const;

    // 获取任务奖励
    int getQuestReward() const;

    // 获取当前任务进度
    int getQuestProgress() const;

    // 获取任务总阶段数
    size_t getTotalStages() const;

    // 获取当前阶段描述
    std::string getCurrentStageDescription() const;

    // 检查当前阶段是否完成
    bool isCurrentStageComplete(const Backpack* backpack, const cocos2d::Vec2& playerPosition) const;

    // 检查任务是否完成
    bool isQuestComplete() const;

    // 进入下一阶段
    bool advanceProgress();

    // 添加阶段
    void addStage(const QuestStage& stage);

    // 获取指定阶段的目标信息
    const QuestStage& getStage(size_t stageIndex) const;

    // 从 JSON 文件加载任务
    void loadFromJson(const std::string& jsonFilePath);

private:
    int questId;                                 // 任务 ID
    int questProgress;                           // 当前任务进度（从 0 开始，表示当前阶段）
    bool questComplete;                          // 任务是否完成
    std::vector<QuestStage> stages;              // 任务阶段信息
    int reward;                                 // 货币奖励数
};

#endif  // __QUEST_H__
