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
    ObjectType type;           // ��������
    std::string imagePath;     // ��������·��
    bool spriteGenerated;      // �Ƿ��Ѿ����ɾ���

public:
    // Ĭ�Ϲ��캯��
    SceneObject();

    // ���������캯��
    SceneObject(ObjectType type, const cocos2d::Vec2& position, const std::string& image);

    // ��ȡ��������
    ObjectType getType() const;

    // ��ȡ���������·��
    const std::string& getImagePath() const;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString);

    // ����Ҫʱ���ɾ���
    void generateSpriteIfNeeded();

    // ��¡���壨����������ʵ����
    SceneObject* clone(const cocos2d::Vec2& position) const;
};

#endif // SCENEOBJECT_H
