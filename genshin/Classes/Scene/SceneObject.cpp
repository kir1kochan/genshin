#include "SceneObject.h"
#include "cocos2d.h"
#include "rapidjson/document.h"
#include <fstream>

using namespace cocos2d;

// 默认构造函数
SceneObject::SceneObject()
    : type(ObjectType::TREE), spriteGenerated(false) {
    // 这里可以初始化为默认值，也可以为空
}

// 带参数构造函数
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

// 克隆物体（用于生成新实例）
SceneObject* SceneObject::clone(const cocos2d::Vec2& position) const {
    return new SceneObject(type, position, imagePath);
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
        if (typeStr == "tree") {
            type = ObjectType::TREE;
        }
        else if (typeStr == "building") {
            type = ObjectType::BUILDING;
        }
        else if (typeStr == "item") {
            type = ObjectType::ITEM;
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

// 在需要时生成精灵
void SceneObject::generateSpriteIfNeeded() {
    if (!spriteGenerated) {
        auto sprite = Sprite::create(imagePath);
        if (sprite) {
            this->addChild(sprite);  // 将精灵添加到场景物体中
            spriteGenerated = true;   // 标记精灵已生成
            auto physicsBody = PhysicsBody::createBox(Size(50, 50));  // 假设是一个50x50的矩形
            physicsBody->setDynamic(true);  // 设置为动态物理体
            physicsBody->setContactTestBitmask(0xFFFFFFFF);  // 设置碰撞测试掩码
            setPhysicsBody(physicsBody);  // 将物理体添加到节点
        }
    }
}
