#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include "../../System/FishingSystem/FishingSystem.h"
#include "../../Common/Backpack/Backpack.h"
enum class ObjectType {
    PICKUP,   // 采摘
    COOKING,  // 烹饪
    FISHING   // 钓鱼
};

// 将字符串转换为 ObjectType 枚举
ObjectType stringToObjectType(const std::string& str);

class SceneObject : public cocos2d::Node {
public:
    
private:
    ObjectType type;               // 物体类型
    std::string imagePath;         // 物体纹理路径
    bool spriteGenerated;          // 是否已经生成精灵 
    std::vector<int> itemIds;      // 采摘物品和所能掉到物品的ID 采摘点只存一个物品并且在采摘后归0不添加，地图刷新后重置
                                   // 钓鱼点存一系列物品，钓鱼成功随机添加其中一个

public:
    // 默认构造函数
    SceneObject();

    // 带参数构造函数
    SceneObject(ObjectType type, const cocos2d::Vec2& position, const std::string& image);

    // 获取物体类型
    ObjectType getType() const;

    // 获取物体的纹理路径
    const std::string& getImagePath() const;

    // 添加物品ID（对于采摘类型）
    void addItemId(int itemId);

    // 获取物品ID（对于采摘类型）
    const std::vector<int>& getItemIds() const;

    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString);

    // 触发交互事件，返回交互结果
    void interactWithPlayer(Backpack* backpack);

    // 克隆物体（用于生成新实例）
    SceneObject* clone(const cocos2d::Vec2& position) const;

    // 在需要时生成精灵
    void generateSpriteIfNeeded();
};

#endif // SCENEOBJECT_H
