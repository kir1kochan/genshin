#include "TreasureChest.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>

TreasureChest::TreasureChest() = default;

TreasureChest::~TreasureChest() = default;

void TreasureChest::setItemInChest(const cocos2d::Vec2& chestPosition, int itemID) {
    chestContents[chestPosition] = itemID;
    if (std::find(chestPositions.begin(), chestPositions.end(), chestPosition) == chestPositions.end()) {
        chestPositions.push_back(chestPosition);
    }
    CCLOG("Item with ID %d set in chest at (%.2f, %.2f).", itemID, chestPosition.x, chestPosition.y);
}

void TreasureChest::openChest(Backpack& backpack, const cocos2d::Vec2& chestPosition) {
    if (chestContents.find(chestPosition) != chestContents.end()) {
        int itemID = chestContents[chestPosition];
        if (itemID != 0) {
            backpack.addItem(itemID,1); 
            CCLOG("Player collected item with ID %d from chest at (%.2f, %.2f).", itemID, chestPosition.x, chestPosition.y);
        }
        else {
            CCLOG("Chest at (%.2f, %.2f) is empty.", chestPosition.x, chestPosition.y);
        }
        chestContents[chestPosition] = 0;  // 清空宝箱
    }
    CCLOG("No chest found at (%.2f, %.2f).", chestPosition.x, chestPosition.y);
}

void TreasureChest::loadFromJson(const std::string& jsonFilePath) {
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

        // 从 JSON 加载宝箱数据
        if (doc.HasMember("chests") && doc["chests"].IsArray()) {
            const rapidjson::Value& chests = doc["chests"];
            for (rapidjson::SizeType i = 0; i < chests.Size(); ++i) {
                const rapidjson::Value& chest = chests[i];
                float x = chest["x"].GetFloat();
                float y = chest["y"].GetFloat();
                int itemID = chest["itemID"].GetInt();

                cocos2d::Vec2 position(x, y);
                chestPositions.push_back(position);
                chestContents[position] = itemID;
            }
        }
        CCLOG("Loaded treasure chests from JSON file: %s", jsonFilePath.c_str());
    }
    else {
        CCLOG("Failed to open JSON file: %s", jsonFilePath.c_str());
    }
}

void TreasureChest::saveToJson(const std::string& jsonFilePath) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 保存宝箱数据
    rapidjson::Value chests(rapidjson::kArrayType);
    for (const auto& position : chestPositions) {
        rapidjson::Value chestObj(rapidjson::kObjectType);
        chestObj.AddMember("x", position.x, allocator);
        chestObj.AddMember("y", position.y, allocator);
        chestObj.AddMember("itemID", chestContents[position], allocator);
        chests.PushBack(chestObj, allocator);
    }
    doc.AddMember("chests", chests, allocator);

    // 将数据保存为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    // 写入到文件
    std::ofstream file(jsonFilePath);
    if (file.is_open()) {
        file << buffer.GetString();
        file.close();
        CCLOG("Saved treasure chests to JSON file: %s", jsonFilePath.c_str());
    }
    else {
        CCLOG("Failed to save to JSON file: %s", jsonFilePath.c_str());
    }
}
