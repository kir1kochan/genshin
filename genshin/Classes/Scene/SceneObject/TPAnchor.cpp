#include "TPAnchor.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <sstream>

TPAnchor::TPAnchor() = default;

TPAnchor::~TPAnchor() = default;

void TPAnchor::teleport(Player& player) {
    // 选择一个激活的 TP 点并传送玩家
    for (const auto& point : tpPoints) {
        if (tpPointActivation[point]) {
            player.setPosition(point);  // 传送玩家到 TP 点
            CCLOG("Player teleported to: (%.2f, %.2f)", point.x, point.y);
            return;
        }
    }

    CCLOG("No activated TP point found.");
}

void TPAnchor::activateTPPoint(const cocos2d::Vec2& point) {
    tpPointActivation[point] = true;  // 激活某个点
    CCLOG("TP Point (%.2f, %.2f) activated.", point.x, point.y);

    // 获取 HUD 对象并隐藏雾图层
    auto hud = dynamic_cast<Hud*>(this->getParent()->getChildByName("Hud"));
    if (hud) {
        hud->hideFogLayers(hud->getMiniMapNode());
        if (hud->getExpandedMiniMapNode()) {
            hud->hideFogLayers(hud->getExpandedMiniMapNode());
        }
    }
}

void TPAnchor::loadFromJson(const std::string& jsonFilePath) {
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

        // 从 JSON 加载 TP 点位和激活情况
        if (doc.HasMember("tpPoints") && doc["tpPoints"].IsArray()) {
            const rapidjson::Value& points = doc["tpPoints"];
            for (rapidjson::SizeType i = 0; i < points.Size(); ++i) {
                const rapidjson::Value& point = points[i];
                float x = point["x"].GetFloat();
                float y = point["y"].GetFloat();

                tpPoints.push_back(cocos2d::Vec2(x, y));

                // 默认未激活, 但是如果 JSON 中有激活状态，就恢复激活状态
                bool isActivated = false;
                if (point.HasMember("activated")) {
                    isActivated = point["activated"].GetBool();
                }
                tpPointActivation[cocos2d::Vec2(x, y)] = isActivated;
            }
        }
    }
    else {
        CCLOG("Failed to open JSON file.");
    }
}


void TPAnchor::saveToJson(const std::string& jsonFilePath) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 保存 TP 点位和激活情况
    rapidjson::Value points(rapidjson::kArrayType);
    for (const auto& point : tpPoints) {
        rapidjson::Value pointObj(rapidjson::kObjectType);
        pointObj.AddMember("x", point.x, allocator);
        pointObj.AddMember("y", point.y, allocator);

        // 添加激活状态
        if (tpPointActivation.find(point) != tpPointActivation.end()) {
            pointObj.AddMember("activated", tpPointActivation.at(point), allocator);
        }
        points.PushBack(pointObj, allocator);
    }
    doc.AddMember("tpPoints", points, allocator);

    // 将数据保存为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    // 写入到文件
    std::ofstream file(jsonFilePath);
    if (file.is_open()) {
        file << buffer.GetString();
        file.close();
        CCLOG("Saved TP points to: %s", jsonFilePath.c_str());
    }
    else {
        CCLOG("Failed to save to JSON file.");
    }
}

void TPAnchor::hideFogLayers(cocos2d::TMXTiledMap* map) {
    for (int i = 1; i <= 5; ++i) {
        std::string layerName = "fog_" + std::to_string(i);
        auto layer = map->getLayer(layerName);
        if (layer) {
            layer->setVisible(false);
        }
    }
}
