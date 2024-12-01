#include "SceneObject.h"
#include "cocos2d.h"
#include "rapidjson/document.h"
#include <fstream>

using namespace cocos2d;

// Ĭ�Ϲ��캯��
SceneObject::SceneObject()
    : type(ObjectType::TREE), spriteGenerated(false) {
    // ������Գ�ʼ��ΪĬ��ֵ��Ҳ����Ϊ��
}

// ���������캯��
SceneObject::SceneObject(ObjectType type, const Vec2& position, const std::string& image)
    : type(type), imagePath(image), spriteGenerated(false) {
    setPosition(position);  // ���������λ��
}

// ��ȡ��������
SceneObject::ObjectType SceneObject::getType() const {
    return type;
}

// ��ȡ���������·��
const std::string& SceneObject::getImagePath() const {
    return imagePath;
}

// ��¡���壨����������ʵ����
SceneObject* SceneObject::clone(const cocos2d::Vec2& position) const {
    return new SceneObject(type, position, imagePath);
}

// �� JSON �ַ�����������
void SceneObject::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        CCLOG("Error parsing JSON string.");
        return;
    }

    // ��ȡ��������
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

    // ��������λ��
    if (doc.HasMember("position") && doc["position"].IsObject()) {
        float x = doc["position"]["x"].GetFloat();
        float y = doc["position"]["y"].GetFloat();
        setPosition(Vec2(x, y));
    }

    // ��������ͼƬ·��
    if (doc.HasMember("image")) {
        imagePath = doc["image"].GetString();
    }
}

// ����Ҫʱ���ɾ���
void SceneObject::generateSpriteIfNeeded() {
    if (!spriteGenerated) {
        auto sprite = Sprite::create(imagePath);
        if (sprite) {
            this->addChild(sprite);  // ��������ӵ�����������
            spriteGenerated = true;   // ��Ǿ���������
            auto physicsBody = PhysicsBody::createBox(Size(50, 50));  // ������һ��50x50�ľ���
            physicsBody->setDynamic(true);  // ����Ϊ��̬������
            physicsBody->setContactTestBitmask(0xFFFFFFFF);  // ������ײ��������
            setPhysicsBody(physicsBody);  // ����������ӵ��ڵ�
        }
    }
}
