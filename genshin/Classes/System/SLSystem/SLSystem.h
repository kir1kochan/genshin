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

    // Main progress
    void setMainProgress(int major, int minor);
    void getMainProgress(int& major, int& minor) const;

    // Side quest status
    void setSideQuestStatus(int questId, bool isCompleted);
    bool getSideQuestStatus(int questId) const;

    // Player
    void setPlayer(Player* player);
    Player* getPlayer() const;

    // Player position
    void setPlayerPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPlayerPosition() const;

    // Treasure chest manager
    void setTreasureChestManager(TreasureChest* treasureChest);
    TreasureChest* getTreasureChestManager() const;

    // TPAnchor
    void setTPAnchor(TPAnchor* tpAnchor);
    TPAnchor* getTPAnchor() const;

    // Save & Load
    void saveToJson(const std::string& jsonFilePath) const;
    void loadFromJson(const std::string& jsonFilePath);

private:
    // Main progress tracking
    int mainMajorProgress;
    int mainMinorProgress;

    // Side quest statuses
    std::unordered_map<int, bool> sideQuestStatus;

    // Player-related data
    Player* player;
    cocos2d::Vec2 playerPosition;

    // Other managers
    TreasureChest* treasureChestManager;
    TPAnchor* tpAnchor;
};

#endif // SLSYSTEM_H
