#include "BackpackLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

BackpackLayer::BackpackLayer() {}

BackpackLayer::~BackpackLayer() {}

bool BackpackLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    for (int i = 1; i < 63; i++) {
        placeToId[i] = 0;
    }
    return true;
}



void BackpackLayer::setPlayer(Player* player) {
    this->player = player;
    if (player) {
        backpack = player->getBackpack();
        
    }
}

void BackpackLayer::createItemIcon() {
    this->removeAllChildren();
    // ��Ʒ���ӵ���ʼλ��
    float startX = 700.0f;
    float startY = 960.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 9;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;


    int i = 1;
    auto mainLayer = dynamic_cast<BackpackMainLayer*>(this->getParent());
    const auto& items = backpack->getItems();
    for (const auto& item : items) {
        if (item.second == 0)
            continue;
        Item* theItem = item.first.get();
        std::string filePathJpg = "Icon/" + theItem->getName() + ".jpg";
        std::string filePathPng = "Icon/" + theItem->getName() + ".png";
        std::string filePath=filePathJpg;
        if (!FileUtils::getInstance()->isFileExist(filePathJpg)) {
            filePath = filePathPng;
        }
        auto sprite = Sprite::create(filePath);
        if (sprite) {
            if (item.second != 0) {
                // ���㵱ǰ���ӵ�λ��
                float x = startX + (i % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
                float y = startY - (i / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5
                // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
                float width = sprite->getContentSize().width;
                float height = sprite->getContentSize().height;

                // ����ϳ��ߵ����ű���
                float scale = 72.0f / std::max(width, height);
                int id = item.first.get()->getId();
                // ��ʾ��Ʒ�����ƺ�Ч��
                int type = id / 100000;
                int subType = id % 1000 / 100;
                // ���ݽϳ��ߵ����ű������þ��������
                sprite->setScale(scale);
                sprite->setPosition(Vec2(x, y));
                sprite->setTag(item.first.get()->getId());
                this->addChild(sprite);
                placeToId[i] = item.first.get()->getId();
                switch (type) {
                case 1: // װ����                    
                    switch (subType) {
                    case 1: {
                        auto equipment = std::dynamic_pointer_cast<Weapon>(item.first);
                        mainLayer->addHoverListenerForIcons(sprite, equipment->getName(), std::to_string(equipment->getPower()),
                            equipment->getId(), [this, equipment, sprite, id, mainLayer]() {player->equipWeapon(equipment); this->uploadNumById(id);
                        backpack->removeItem(id); mainLayer->refreshEquipmentIcons();  this->uploadNumById(id);  mainLayer->updatePlayerData(); }, 1);
                        break;
                    }
                    case 2: {
                        auto equipment = std::dynamic_pointer_cast<Armor>(item.first);
                        mainLayer->addHoverListenerForIcons(sprite, equipment->getName(), std::to_string(equipment->getPower()),
                            equipment->getId(), [this, equipment, sprite, id, mainLayer]() {player->equipArmor(equipment); this->uploadNumById(id);
                        backpack->removeItem(id); mainLayer->refreshEquipmentIcons(); this->uploadNumById(id);  mainLayer->updatePlayerData(); }, 1);
                        break;
                    }
                    case 3: {
                        auto equipment = std::dynamic_pointer_cast<Accessory>(item.first);
                        mainLayer->addHoverListenerForIcons(sprite, equipment->getName(), std::to_string(equipment->getPower()),
                            equipment->getId(), [this, equipment, sprite, id, mainLayer]() {player->equipAccessory(equipment); this->uploadNumById(id);
                        backpack->removeItem(id); mainLayer->refreshEquipmentIcons();  this->uploadNumById(id);  mainLayer->updatePlayerData(); }, 1);
                        break;
                    }
                    default:
                        CCLOG("Error: Unsupported equipment subtype %d for ID %d", subType, id);
                        break;
                    }
                    break;
                case 2: { // ҩ����
                    auto potion = std::dynamic_pointer_cast<Potion>(item.first);
                    mainLayer->addHoverListenerForIcons(sprite, potion->getName(), std::to_string((int)potion->getHealAmount()),
                        potion->getId(), [this, potion, sprite, id, mainLayer]() {potion.get()->use(*player); this->uploadNumById(id) ; backpack->removeItem(id);
                    mainLayer->updatePlayerData(); },1);
                    break;
                }
                case 3: {// ʳ����
                    auto food = std::dynamic_pointer_cast<Food>(item.first);
                    mainLayer->addHoverListenerForIcons(sprite, food->getName(), std::to_string((int)food->getHealAmount()),
                        food->getId(), [this, food, sprite, id, mainLayer]() {food.get()->use(player); this->uploadNumById(id) ; backpack->removeItem(id);
                    mainLayer->updatePlayerData(); },1);
                    break;
                }
                default:
                    CCLOG("Error: Unsupported type %d for ID %d", type, id);
                    break;
                }
                i++;
            }
        }
    }
    i = 0;
    int coins = backpack->getCoins();
    float x = startX + (i % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
    float y = startY - (i / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5
    auto sprite = Sprite::create("Icon/coin.png");
    // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
    float width = sprite->getContentSize().width;
    float height = sprite->getContentSize().height;

    // ����ϳ��ߵ����ű���
    float scale = 0.5;
    sprite->setScale(scale);
    sprite->setPosition(Vec2(x, y));
    // ����һ����ʾ���ֵ� Label�����������ı�
    auto label = Label::createWithTTF(std::to_string(coins), "fonts/arial.ttf", 24); // ʹ�ú��ʵ�����ʹ�С
    label->setTextColor(Color4B::WHITE); // ����������ɫ

    label->setPosition(Vec2(x + 30, y - 30));

    this->addChild(label);
    this->addChild(sprite);
}

void BackpackLayer::createNums() {
    // ��Ʒ���ӵ���ʼλ��
    float startX = 700.0f;
    float startY = 960.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 9;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;

    int nums;
    for (const auto& i : placeToId) {
        if (i.second != 0) {
            nums = backpack->getItemCountById(i.second);
            float x = startX + (i.first % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
            float y = startY - (i.first / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5
            if (nums != 0) {
                // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
                // ����һ����ʾ���ֵ� Label�����������ı�
                auto label = Label::createWithTTF(std::to_string(nums), "fonts/arial.ttf", 24); // ʹ�ú��ʵ�����ʹ�С
                label->setTextColor(Color4B::WHITE); // ����������ɫ

                label->setPosition(Vec2(x + 30, y - 30));
                label->setTag(i.second + 1000000);
                this->addChild(label);
            }
        }
    }
}

void BackpackLayer::uploadNumById(int id) {
    // ��Ʒ���ӵ���ʼλ��
    float startX = 700.0f;
    float startY = 960.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 9;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;

    int nums;
    for (auto& i : placeToId) {
        if (i.second == id) {
            nums = backpack->getItemCountById(i.second);
            float x = startX + (i.first % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
            float y = startY - (i.first / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5
            if (nums != 0) {
                this->removeChildByTag(id + 1000000);
                // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
                // ����һ����ʾ���ֵ� Label�����������ı�
                auto label = Label::createWithTTF(std::to_string(nums), "fonts/arial.ttf", 24); // ʹ�ú��ʵ�����ʹ�С
                label->setTextColor(Color4B::WHITE); // ����������ɫ

                label->setPosition(Vec2(x + 30, y - 30));
                label->setTag(i.second + 1000000);
                this->addChild(label);
            }
            else{
                auto mainLayer = dynamic_cast<BackpackMainLayer*>(this->getParent());
                mainLayer->eraseHoverListenerForIcons(dynamic_cast<Sprite*>(this->getChildByTag(id)),1);
                this->removeChildByTag(id + 1000000);
                this->removeChildByTag(id);
                i.second = 0;
            }
        }
    }
}

void BackpackLayer::addItemToBlank(int id) {
    // ��Ʒ���ӵ���ʼλ��
    float startX = 700.0f;
    float startY = 960.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 9;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;
    auto mainLayer = dynamic_cast<BackpackMainLayer*>(this->getParent());
    int nums;
    backpack->addItem(id);
    for (const auto& i : placeToId) {
        if (i.second == 0) {
            nums = backpack->getItemCountById(id);
            float x = startX + (i.first % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
            float y = startY - (i.first / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5
            if (nums != 0) {
                auto item = backpack->findItemById(id);
                std::string filePathJpg = "Icon/" + item.get()->getName() + ".jpg";
                auto sprite = Sprite::create(filePathJpg);
                int thepos = i.first;
                // ���㵱ǰ���ӵ�λ��
                float x = startX + (thepos % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
                float y = startY - (thepos / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5
                // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
                float width = sprite->getContentSize().width;
                float height = sprite->getContentSize().height;

                // ����ϳ��ߵ����ű���
                float scale = 72.0f / std::max(width, height);
                // ��ʾ��Ʒ�����ƺ�Ч��
                int type = id / 100000;
                int subType = id % 1000 / 100;
                // ���ݽϳ��ߵ����ű������þ��������
                sprite->setScale(scale);
                sprite->setPosition(Vec2(x, y));
                sprite->setTag(id);
                this->addChild(sprite);
                placeToId[thepos] = id;
                switch (type) {
                case 1: // װ����                    
                    switch (subType) {
                    case 1: {
                        auto equipment = std::dynamic_pointer_cast<Weapon>(item);
                        mainLayer->addHoverListenerForIcons(sprite, equipment->getName(), std::to_string(equipment->getPower()),
                            equipment->getId(), [this, equipment, sprite, id, mainLayer]() {player->equipWeapon(equipment); this->uploadNumById(id);
                        backpack->removeItem(id); mainLayer->refreshEquipmentIcons();  this->uploadNumById(id);  mainLayer->updatePlayerData(); }, 1);
                        break;
                    }
                    case 2: {
                        auto equipment = std::dynamic_pointer_cast<Armor>(item);
                        mainLayer->addHoverListenerForIcons(sprite, equipment->getName(), std::to_string(equipment->getPower()),
                            equipment->getId(), [this, equipment, sprite, id, mainLayer]() {player->equipArmor(equipment); this->uploadNumById(id);
                        backpack->removeItem(id); mainLayer->refreshEquipmentIcons();  this->uploadNumById(id); mainLayer->updatePlayerData(); }, 1);
                        break;
                    }
                    case 3: {
                        auto equipment = std::dynamic_pointer_cast<Accessory>(item);
                        mainLayer->addHoverListenerForIcons(sprite, equipment->getName(), std::to_string(equipment->getPower()),
                            equipment->getId(), [this, equipment, sprite, id, mainLayer]() {player->equipAccessory(equipment); this->uploadNumById(id);
                        backpack->removeItem(id); mainLayer->refreshEquipmentIcons();  this->uploadNumById(id); mainLayer->updatePlayerData(); }, 1);
                        break;
                    }
                    default:
                        CCLOG("Error: Unsupported equipment subtype %d for ID %d", subType, id);
                        break;
                    }
                    break;
                default:
                    CCLOG("Error: Unsupported type %d for ID %d", type, id);
                    break;
                }
                uploadNumById(id);
                break;
            }
            
        }
    }
}
