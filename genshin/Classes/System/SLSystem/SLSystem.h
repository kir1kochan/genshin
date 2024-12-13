#ifndef SLSYSTEM_H
#define SLSYSTEM_H

#include "Player.h"
#include "TreasureChest.h"
#include "TPAnchor.h"
#include "cocos2d.h"
#include <string>
#include <unordered_map>

class SLSystem {
public:
    SLSystem();
    ~SLSystem();

    // �趨�Ͷ�ȡ��������Ľ���
    void setMainProgress(int major, int minor);
    void getMainProgress(int& major, int& minor) const;

    // �趨�Ͷ�ȡ֧�������������
    void setSideQuestStatus(int questId, bool isCompleted);
    bool getSideQuestStatus(int questId) const;

    // �趨�Ͷ�ȡ�����ָ��
    void setPlayer(Player* player);
    Player* getPlayer() const;

    // �趨�Ͷ�ȡ�����õ����λ��
    void setPlayerPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPlayerPosition() const;

    // �趨�Ͷ�ȡ������ָ��
    void setTreasureChest(TreasureChest* treasureChest);
    TreasureChest* getTreasureChest() const;

    // �趨�Ͷ�ȡê����ָ��
    void setTPAnchor(TPAnchor* tpAnchor);
    TPAnchor* getTPAnchor() const;

    // �����������������Ҫһ����������ݵı���Ͷ�ȡ
    void saveToJson(const std::string& jsonFilePath) const;
    void loadFromJson(const std::string& jsonFilePath);

private:
    // ��������Ľ���
    int mainMajorProgress;
    int mainMinorProgress;

    // ֧�������������
    std::unordered_map<int, bool> sideQuestStatus;

    // �����ָ��
    Player* player;

    cocos2d::Vec2 playerPosition;

    // ������ָ��
    TreasureChest* treasureChest;

    // ê����ָ��
    TPAnchor* tpAnchor;
};

#endif // SLSYSTEM_H
