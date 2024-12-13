#ifndef __QUEST_H__
#define __QUEST_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Backpack.h" // ���� Backpack ���Ѿ�����

struct QuestStage {
    std::unordered_map<int, int> requiredItems; // ÿ���׶���Ҫ����Ʒ�������� (itemId, quantity)
    cocos2d::Vec2 targetPosition;              // ÿ���׶ε�Ŀ��λ��
    std::string description;                   // �׶���������
};

class Quest {
public:
    Quest(int id);
    ~Quest();

    // ��ȡ���� ID
    int getQuestId() const;

    // ��ȡ������
    int getQuestReward() const;

    // ��ȡ��ǰ�������
    int getQuestProgress() const;

    // ��ȡ�����ܽ׶���
    size_t getTotalStages() const;

    // ��ȡ��ǰ�׶�����
    std::string getCurrentStageDescription() const;

    // ��鵱ǰ�׶��Ƿ����
    bool isCurrentStageComplete(const Backpack* backpack, const cocos2d::Vec2& playerPosition) const;

    // ��������Ƿ����
    bool isQuestComplete() const;

    // ������һ�׶�
    bool advanceProgress();

    // ��ӽ׶�
    void addStage(const QuestStage& stage);

    // ��ȡָ���׶ε�Ŀ����Ϣ
    const QuestStage& getStage(size_t stageIndex) const;

    // �� JSON �ļ���������
    void loadFromJson(const std::string& jsonFilePath);

private:
    int questId;                                 // ���� ID
    int questProgress;                           // ��ǰ������ȣ��� 0 ��ʼ����ʾ��ǰ�׶Σ�
    bool questComplete;                          // �����Ƿ����
    std::vector<QuestStage> stages;              // ����׶���Ϣ
    int reward;                                 // ���ҽ�����
};

#endif  // __QUEST_H__
