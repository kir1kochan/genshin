#include "SceneObject.h"
#include "cocos2d.h"
#include "json/document.h"
#include <fstream>
#include "../../Common/Backpack/Backpack.h"

using namespace cocos2d;

// ���ַ���ת��Ϊ ObjectType ö��
ObjectType stringToObjectType(const std::string& str) {
    if (str == "PICKUP"||str=="Collection") {
        return ObjectType::PICKUP;
    }
    else if (str == "COOKING") {
        return ObjectType::COOKING;
    }
    else if (str == "FISHING"|| str == "Fish") {
        return ObjectType::FISHING;
    }
    else {
        throw std::invalid_argument("Invalid ObjectType string: " + str);
    }
}

SceneObject::SceneObject()
    : type(ObjectType::PICKUP), spriteGenerated(false) {
}

SceneObject::SceneObject(ObjectType type, const Vec2& position, const std::string& image)
    : type(type), imagePath(image), spriteGenerated(false) {
    setPosition(position);  // ���������λ��
}

// ��ȡ��������
ObjectType SceneObject::getType() const {
    return type;
}

// ��ȡ���������·��
const std::string& SceneObject::getImagePath() const {
    return imagePath;
}

// �����ƷID�����ڲ�ժ���ͺ͵������ͣ�
void SceneObject::addItemId(int itemId) {
    itemIds.push_back(itemId);
}

// ��ȡ��ƷID�����ڲ�ժ���ͺ͵������ͣ�
const std::vector<int>& SceneObject::getItemIds() const {
    return itemIds;
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

// ���������¼������ؽ������
void SceneObject::interactWithPlayer(Backpack* backpack) {
    switch (type) {
    case ObjectType::PICKUP: {
        // ��ժʱ���������ƷID
        if (!itemIds.empty()) {
            // ����һ�λ��1����Ʒ
            backpack->addItem(itemIds[0], 1);
            itemIds.clear(); // ��ժ�������ƷID
            CCLOG("Item picked up!");
        }
        else {
            CCLOG("No items to pick up.");
        }
        break;
    }
    case ObjectType::COOKING: {
        // ��⿽������������ҳ�沢������⿽��
        CCLOG("Cooking interaction triggered!");
        // �����������¼�
        auto cookingEvent = new cocos2d::EventCustom("COOKING_STARTED_EVENT");
        _eventDispatcher->dispatchEvent(cookingEvent);  // �����¼�
        break;
    }
    case ObjectType::FISHING: {
        // ���㽻���������������
        srand(time(0)); // ��ʼ���������

        auto fishingEvent = new cocos2d::EventCustom("FISHING_STARTED_EVENT");
        _eventDispatcher->dispatchEvent(fishingEvent);  // �����¼�
        // ��������������ϵͳ
        FishingSystem* fishingSystem = new FishingSystem;
        fishingSystem->startFishing(Director::getInstance()->getRunningScene());

        // ���õ������Ļص�����
        fishingSystem->setOnFishingResultCallback([this, backpack](bool success) {
            if (success) {
                CCLOG("You successfully caught the fish!");
                // ����ɹ������������Ʒ
                if (!itemIds.empty()) {
                    int randomIndex = rand() % itemIds.size();  // ����ƷID�б������ѡ��һ��
                    int caughtItemId = itemIds[randomIndex];
                    backpack->addItem(caughtItemId, 1);  // ����������Ʒ��ӵ�������
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

// ��¡���壨����������ʵ����
SceneObject* SceneObject::clone(const Vec2& position) const {
    return new SceneObject(type, position, imagePath);
}

// ����Ҫʱ���ɾ���
void SceneObject::generateSpriteIfNeeded() {
    if (!spriteGenerated) {
        if (type == ObjectType::PICKUP) {
            auto sprite = Sprite::create("food/Apple.jpg");
            sprite->setContentSize(cocos2d::Size(72, 72));
            sprite->setName("sprite");
            auto runningScene = Director::getInstance()->getRunningScene();

            if (sprite) {
                this->addChild(sprite);  // ��������ӵ�����������
                runningScene->addChild(this);
                spriteGenerated = true;   // ��Ǿ���������
            }
        }
        spriteGenerated = true;   // ��Ǿ���������
    }
}
