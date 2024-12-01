#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "cocos2d.h"
#include <string>
#include <unordered_map>

class SceneObject : public cocos2d::Node {
public:
    enum class ObjectType {
        TREE,
        BUILDING,
        ITEM
    };

private:
    ObjectType type;           // 物体类型
    std::string imagePath;     // 物体纹理路径
    bool spriteGenerated;      // 是否已经生成精灵

public:
    // 默认构造函数
    SceneObject();

    // 带参数构造函数
    SceneObject(ObjectType type, const cocos2d::Vec2& position, const std::string& image);

    // 获取物体类型
    ObjectType getType() const;

    // 获取物体的纹理路径
    const std::string& getImagePath() const;

    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString);

    // 在需要时生成精灵
    void generateSpriteIfNeeded();

    // 克隆物体（用于生成新实例）
    SceneObject* clone(const cocos2d::Vec2& position) const;
};

#endif // SCENEOBJECT_H
