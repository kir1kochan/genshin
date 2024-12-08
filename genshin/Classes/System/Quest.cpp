#include "Quest.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <sstream>

Quest::Quest(int id)
    : questId(id), questProgress(0), questComplete(false) {}

Quest::~Quest() = default;

int Quest::getQuestId() const {
    return questId;
}

int Quest::getQuestProgress() const {
    return questProgress;
}

size_t Quest::getTotalStages() const {
    return stages.size();
}

std::string Quest::getCurrentStageDescription() const {
    if (questProgress < stages.size()) {
        return stages[questProgress].description;
    }
    return "Quest complete.";
}

bool Quest::isCurrentStageComplete(const Backpack* backpack, const cocos2d::Vec2& playerPosition) const {
    if (questComplete || questProgress >= stages.size()) {
        return false;
    }

    const QuestStage& currentStage = stages[questProgress];

    // 检查是否所有物品满足要求
    for (const auto& [itemId, quantity] : currentStage.requiredItems) {
        if (!backpack || backpack->getItemCountById(itemId) < quantity) {
            return false;
        }
    }


    return true;
}

bool Quest::isQuestComplete() const {
    return questComplete;
}

bool Quest::advanceProgress() {
    if (questProgress < stages.size()) {
        ++questProgress;
        if (questProgress >= stages.size()) {
            questComplete = true;  // 标记任务完成
        }
        return true;
    }
    return false;
}

void Quest::addStage(const QuestStage& stage) {
    stages.push_back(stage);
}

const QuestStage& Quest::getStage(size_t stageIndex) const {
    if (stageIndex >= stages.size()) {
        throw std::out_of_range("Stage index is out of range");
    }
    return stages[stageIndex];
}

void Quest::loadFromJson(const std::string& jsonFilePath) {
    std::ifstream file(jsonFilePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        std::string jsonString = buffer.str();

        rapidjson::Document doc;
        doc.Parse(jsonString.c_str());

        if (doc.HasParseError() || !doc.IsObject()) {
            CCLOG("Error parsing JSON file.");
            return;
        }

        // 解析任务 ID
        if (doc.HasMember("questId") && doc["questId"].IsInt()) {
            questId = doc["questId"].GetInt();
        }

        // 解析任务阶段
        if (doc.HasMember("stages") && doc["stages"].IsArray()) {
            const rapidjson::Value& stagesArray = doc["stages"];
            for (rapidjson::SizeType i = 0; i < stagesArray.Size(); ++i) {
                const rapidjson::Value& stage = stagesArray[i];

                QuestStage questStage;

                // 解析阶段描述
                if (stage.HasMember("description") && stage["description"].IsString()) {
                    questStage.description = stage["description"].GetString();
                }

                // 解析目标位置
                if (stage.HasMember("targetPosition") && stage["targetPosition"].IsObject()) {
                    const rapidjson::Value& position = stage["targetPosition"];
                    questStage.targetPosition = cocos2d::Vec2(
                        position["x"].GetFloat(),
                        position["y"].GetFloat()
                    );
                }

                // 解析所需物品
                if (stage.HasMember("requiredItems") && stage["requiredItems"].IsArray()) {
                    const rapidjson::Value& items = stage["requiredItems"];
                    for (rapidjson::SizeType j = 0; j < items.Size(); ++j) {
                        const rapidjson::Value& item = items[j];
                        int itemId = item["itemId"].GetInt();
                        int quantity = item["quantity"].GetInt();
                        questStage.requiredItems[itemId] = quantity;
                    }
                }

                addStage(questStage);
            }
        }

        CCLOG("Quest %d loaded with %zu stages.", questId, stages.size());
    }
    else {
        CCLOG("Failed to open JSON file: %s", jsonFilePath.c_str());
    }
}
