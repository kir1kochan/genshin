#include "SLSystem.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace rapidjson;

SLSystem::SLSystem()
    : mainMajorProgress(0),
    mainMinorProgress(0),
    player(nullptr),
    treasureChest(nullptr),
    tpAnchor(nullptr),
    playerPosition(cocos2d::Vec2::ZERO) {}

SLSystem::~SLSystem() {}

void SLSystem::setMainProgress(int major, int minor) {
    mainMajorProgress = major;
    mainMinorProgress = minor;
}

void SLSystem::getMainProgress(int& major, int& minor) const {
    major = mainMajorProgress;
    minor = mainMinorProgress;
}

void SLSystem::setSideQuestStatus(int questId, bool isCompleted) {
    sideQuestStatus[questId] = isCompleted;
}

bool SLSystem::getSideQuestStatus(int questId) const {
    auto it = sideQuestStatus.find(questId);
    return (it != sideQuestStatus.end()) ? it->second : false;
}

void SLSystem::setPlayer(Player* player) {
    this->player = player;
}

Player* SLSystem::getPlayer() const {
    return player;
}

void SLSystem::setTreasureChest(TreasureChest* treasureChest) {
    this->treasureChest = treasureChest;
}

TreasureChest* SLSystem::getTreasureChest() const {
    return treasureChest;
}

void SLSystem::setTPAnchor(TPAnchor* tpAnchor) {
    this->tpAnchor = tpAnchor;
}

TPAnchor* SLSystem::getTPAnchor() const {
    return tpAnchor;
}

void SLSystem::setPlayerPosition(const cocos2d::Vec2& position) {
    playerPosition = position;
}

cocos2d::Vec2 SLSystem::getPlayerPosition() const {
    return playerPosition;
}

void SLSystem::saveToJson(const std::string& jsonFilePath) const {
    Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // Save main progress
    Value mainProgress(kObjectType);
    mainProgress.AddMember("major", mainMajorProgress, allocator);
    mainProgress.AddMember("minor", mainMinorProgress, allocator);
    doc.AddMember("mainProgress", mainProgress, allocator);

    // Save side quests
    Value sideQuests(kObjectType);
    for (const auto& quest : sideQuestStatus) {
        sideQuests.AddMember(Value(std::to_string(quest.first).c_str(), allocator).Move(),
            quest.second, allocator);
    }
    doc.AddMember("sideQuests", sideQuests, allocator);

    // Save player data
    if (player) {
        std::string playerJson = player->saveToJson();
        doc.AddMember("player", Value(playerJson.c_str(), allocator).Move(), allocator);

        // Save player's backpack
        if (player->getBackpack()) {
            std::string backpackJson = player->getBackpack()->saveToJson();
            doc.AddMember("backpack", Value(backpackJson.c_str(), allocator).Move(), allocator);
        }
    }

    // Save treasure chest data
    if (treasureChest) {
        std::string chestJson;
        treasureChest->saveToJson(chestJson);
        doc.AddMember("treasureChest", Value(chestJson.c_str(), allocator).Move(), allocator);
    }

    // Save TPAnchor data
    if (tpAnchor) {
        std::string tpJson;
        tpAnchor->saveToJson(tpJson);
        doc.AddMember("tpAnchor", Value(tpJson.c_str(), allocator).Move(), allocator);
    }

    // Save player position
    Value position(kObjectType);
    position.AddMember("x", playerPosition.x, allocator);
    position.AddMember("y", playerPosition.y, allocator);
    doc.AddMember("playerPosition", position, allocator);

    // Write JSON to file
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::ofstream ofs(jsonFilePath);
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file for saving: " + jsonFilePath);
    }
    ofs << buffer.GetString();
    ofs.close();
}

void SLSystem::loadFromJson(const std::string& jsonFilePath) {
    std::ifstream ifs(jsonFilePath);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file for loading: " + jsonFilePath);
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    Document doc;
    doc.Parse(buffer.str().c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("Failed to parse JSON file: " + jsonFilePath);
    }

    // Load main progress
    if (doc.HasMember("mainProgress")) {
        const Value& mainProgress = doc["mainProgress"];
        if (mainProgress.HasMember("major")) {
            mainMajorProgress = mainProgress["major"].GetInt();
        }
        if (mainProgress.HasMember("minor")) {
            mainMinorProgress = mainProgress["minor"].GetInt();
        }
    }

    // Load side quests
    if (doc.HasMember("sideQuests")) {
        const Value& sideQuests = doc["sideQuests"];
        for (Value::ConstMemberIterator it = sideQuests.MemberBegin(); it != sideQuests.MemberEnd(); ++it) {
            int questId = std::stoi(it->name.GetString());
            bool isCompleted = it->value.GetBool();
            sideQuestStatus[questId] = isCompleted;
        }
    }

    // Load player data
    if (player && doc.HasMember("player")) {
        player->loadFromJson(doc["player"].GetString());

        // Load player's backpack
        if (player->getBackpack() && doc.HasMember("backpack")) {
            player->getBackpack()->loadFromJson(doc["backpack"].GetString());
        }
    }

    // Load treasure chest data
    if (treasureChest && doc.HasMember("treasureChest")) {
        treasureChest->loadFromJson(doc["treasureChest"].GetString());
    }

    // Load TPAnchor data
    if (tpAnchor && doc.HasMember("tpAnchor")) {
        tpAnchor->loadFromJson(doc["tpAnchor"].GetString());
    }

    // Load player position
    if (doc.HasMember("playerPosition")) {
        const Value& position = doc["playerPosition"];
        if (position.HasMember("x") && position.HasMember("y")) {
            playerPosition.x = position["x"].GetFloat();
            playerPosition.y = position["y"].GetFloat();
        }
    }
}
