#include "SceneObject.h"
#include "cocos2d.h"
#include "rapidjson/document.h"
#include <fstream>
#include "Backpack.h"

using namespace cocos2d;

SceneObject::SceneObject()
    : type(ObjectType::PICKUP), spriteGenerated(false) {
}

SceneObject::SceneObject(ObjectType type, const Vec2& position, const std::string& image)
    : type(type), imagePath(image), spriteGenerated(false) {
    setPosition(position);  // 设置物体的位置
}

// 获取物体类型
SceneObject::ObjectType SceneObject::getType() const {
    return type;
}

// 获取物体的纹理路径
const std::string& SceneObject::getImagePath() const {
    return imagePath;
}

// 添加物品ID（对于采摘类型和钓鱼类型）
void SceneObject::addItemId(int itemId) {
    itemIds.push_back(itemId);
}

// 获取物品ID（对于采摘类型和钓鱼类型）
const std::vector<int>& SceneObject::getItemIds() const {
    return itemIds;
}

// 从 JSON 字符串加载数据
void SceneObject::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        CCLOG("Error parsing JSON string.");
        return;
    }

    // 读取物体类型
    if (doc.HasMember("type")) {
        std::string typeStr = doc["type"].GetString();
        if (typeStr == "pickup") {
            type = ObjectType::PICKUP;
        }
        else if (typeStr == "cooking") {
            type = ObjectType::COOKING;
        }
        else if (typeStr == "fishing") {
            type = ObjectType::FISHING;
        }
    }

    // 解析物体位置
    if (doc.HasMember("position") && doc["position"].IsObject()) {
        float x = doc["position"]["x"].GetFloat();
        float y = doc["position"]["y"].GetFloat();
        setPosition(Vec2(x, y));
    }

    // 解析物体图片路径
    if (doc.HasMember("image")) {
        imagePath = doc["image"].GetString();
    }
}

// 触发交互事件，返回交互结果
void SceneObject::interactWithPlayer(Backpack* backpack) {
    switch (type) {
    case ObjectType::PICKUP: {
        // 采摘时，给玩家物品ID
        if (!itemIds.empty()) {
            // 假设一次获得1个物品
            backpack->addItem(itemIds[0], 1);
            itemIds.clear(); // 采摘后清空物品ID
            CCLOG("Item picked up!");
        }
        else {
            CCLOG("No items to pick up.");
        }
        break;
    }
    case ObjectType::COOKING: {
        // 烹饪交互，触发烹饪页面并返回烹饪结果
        CCLOG("Cooking interaction triggered!");
        // 这里可以触发一个新的页面或逻辑来处理烹饪，可以用事件监听的方式传回，页面根据是否可以制作来决定显示情况。
        break;
    }
    case ObjectType::FISHING: {
        // 钓鱼交互，触发钓鱼过程
        srand(time(0)); // 初始化随机种子
        // 创建并启动钓鱼系统
        FishingSystem* fishingSystem = FishingSystem::create();
        fishingSystem->startFishing();

        // 设置钓鱼结果的回调函数
        fishingSystem->setOnFishingResultCallback([this, backpack](bool success) {
            if (success) {
                CCLOG("You successfully caught the fish!");

                // 钓鱼成功，随机掉落物品
                if (!itemIds.empty()) {
                    int randomIndex = rand() % itemIds.size();  // 从物品ID列表中随机选择一个
                    int caughtItemId = itemIds[randomIndex];
                    backpack->addItem(caughtItemId, 1);  // 将钓到的物品添加到背包中
                    CCLOG("Fishing successful! Caught item ID: %d", caughtItemId);
                }
                else {
                    CCLOG("Fishing successful, but no items available to catch.");
                }
            }
            else {
                CCLOG("Fishing failed.");
            }
            });

        break;
    }
    default:
        break;
    }
}

// 克隆物体（用于生成新实例）
SceneObject* SceneObject::clone(const Vec2& position) const {
    return new SceneObject(type, position, imagePath);
}

// 在需要时生成精灵
void SceneObject::generateSpriteIfNeeded() {
    if (!spriteGenerated) {
        auto sprite = Sprite::create(imagePath);
        if (sprite) {
            this->addChild(sprite);  // 将精灵添加到场景物体中
            spriteGenerated = true;   // 标记精灵已生成
        }
    }
}
