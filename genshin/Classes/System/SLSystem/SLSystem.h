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

    // 设定和读取主线任务的进度
    void setMainProgress(int major, int minor);
    void getMainProgress(int& major, int& minor) const;

    // 设定和读取支线任务的完成情况
    void setSideQuestStatus(int questId, bool isCompleted);
    bool getSideQuestStatus(int questId) const;

    // 设定和读取玩家类指针
    void setPlayer(Player* player);
    Player* getPlayer() const;

    // 设定和读取保存用的玩家位置
    void setPlayerPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPlayerPosition() const;

    // 设定和读取宝箱类指针
    void setTreasureChest(TreasureChest* treasureChest);
    TreasureChest* getTreasureChest() const;

    // 设定和读取锚点类指针
    void setTPAnchor(TPAnchor* tpAnchor);
    TPAnchor* getTPAnchor() const;

    // 进行所有类和其他需要一并保存的内容的保存和读取
    void saveToJson(const std::string& jsonFilePath) const;
    void loadFromJson(const std::string& jsonFilePath);

private:
    // 主线任务的进度
    int mainMajorProgress;
    int mainMinorProgress;

    // 支线任务的完成情况
    std::unordered_map<int, bool> sideQuestStatus;

    // 玩家类指针
    Player* player;

    cocos2d::Vec2 playerPosition;

    // 宝箱类指针
    TreasureChest* treasureChest;

    // 锚点类指针
    TPAnchor* tpAnchor;
};

#endif // SLSYSTEM_H
