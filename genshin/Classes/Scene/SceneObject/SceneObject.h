#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include "../../System/FishingSystem/FishingSystem.h"
#include "../../Common/Backpack/Backpack.h"
enum class ObjectType {
    PICKUP,   // ��ժ
    COOKING,  // ���
    FISHING   // ����
};

// ���ַ���ת��Ϊ ObjectType ö��
ObjectType stringToObjectType(const std::string& str);

class SceneObject : public cocos2d::Node {
public:
    
private:
    ObjectType type;               // ��������
    std::string imagePath;         // ��������·��
    bool spriteGenerated;          // �Ƿ��Ѿ����ɾ��� 
    std::vector<int> itemIds;      // ��ժ��Ʒ�����ܵ�����Ʒ��ID ��ժ��ֻ��һ����Ʒ�����ڲ�ժ���0����ӣ���ͼˢ�º�����
                                   // ������һϵ����Ʒ������ɹ�����������һ��

public:
    // Ĭ�Ϲ��캯��
    SceneObject();

    // ���������캯��
    SceneObject(ObjectType type, const cocos2d::Vec2& position, const std::string& image);

    // ��ȡ��������
    ObjectType getType() const;

    // ��ȡ���������·��
    const std::string& getImagePath() const;

    // �����ƷID�����ڲ�ժ���ͣ�
    void addItemId(int itemId);

    // ��ȡ��ƷID�����ڲ�ժ���ͣ�
    const std::vector<int>& getItemIds() const;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString);

    // ���������¼������ؽ������
    void interactWithPlayer(Backpack* backpack);

    // ��¡���壨����������ʵ����
    SceneObject* clone(const cocos2d::Vec2& position) const;

    // ����Ҫʱ���ɾ���
    void generateSpriteIfNeeded();
};

#endif // SCENEOBJECT_H
