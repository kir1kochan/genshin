#include "BackpackScene.h"
#include "BackpackLayer.h"
#include "SkillLayer.h"
#include "EquipmentLayer.h"
#include "../../Common/Entities/Player/Player.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

BackpackScene::BackpackScene()
    : player(nullptr), backpackLayer(nullptr), skillLayer(nullptr), equipmentLayer(nullptr),
    nextButton(nullptr), prevButton(nullptr) {}

BackpackScene::~BackpackScene() {}

BackpackScene* BackpackScene::createScene(Player* player) {
    // �������������������󣬴����������
    auto scene = BackpackScene::create();
    scene->setPlayer(player);
    return scene;
}

bool BackpackScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ������ɫ����Ϊ��ɫ
    auto background = LayerColor::create(Color4B(139, 69, 19, 255)); // ��ɫ����
    this->addChild(background);

    /*
    // ��������ʼ����������
    backpackLayer = BackpackLayer::create();
    this->addChild(backpackLayer);

    // ��������ʼ�����ܽ���
    skillLayer = SkillLayer::create();
    this->addChild(skillLayer);

    // ��������ʼ��װ������
    equipmentLayer = EquipmentLayer::create();
    this->addChild(equipmentLayer);

    // ��ʼ����Ϊ��ʾ��������
    switchToBackpackLayer();
    */

    // �����̶������������ס���Ʒ����
    createEquipmentGrid();

    // ��������������
    createSkillGrid();

    // �������������ʾ�����顢�ȼ��������ȣ�
    createPlayerDataDisplay();

    /*
    // ��������ʼ���л�����һ�����水ť
    nextButton = Button::create("next_button.png");
    nextButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 50, Director::getInstance()->getVisibleSize().height / 2));
    // �����л�����һ������ĵ���¼�
    nextButton->addClickEventListener([=](Ref* sender) {
        if (backpackLayer->isVisible()) {
            switchToSkillLayer();
        }
        else if (skillLayer->isVisible()) {
            switchToEquipmentLayer();
        }
        else {
            switchToBackpackLayer();
        }
        });
    this->addChild(nextButton);

    // ��������ʼ���л�����һ�����水ť
    prevButton = Button::create("prev_button.png");
    prevButton->setPosition(Vec2(50, Director::getInstance()->getVisibleSize().height / 2));
    prevButton->addClickEventListener([=](Ref* sender) {
        if (backpackLayer->isVisible()) {
            switchToEquipmentLayer();
        }
        else if (equipmentLayer->isVisible()) {
            switchToSkillLayer();
        }
        else {
            switchToBackpackLayer();
        }
        });
    this->addChild(prevButton);
    */

    return true;
}

void BackpackScene::setPlayer(Player* player) {
    this->player = player;
}

Player* BackpackScene::getPlayer() const {
    return player;
}

void BackpackScene::switchToBackpackLayer() {
    backpackLayer->setVisible(true);
    skillLayer->setVisible(false);
    equipmentLayer->setVisible(false);
}

void BackpackScene::switchToSkillLayer() {
    backpackLayer->setVisible(false);
    skillLayer->setVisible(true);
    equipmentLayer->setVisible(false);
}

void BackpackScene::switchToEquipmentLayer() {
    backpackLayer->setVisible(false);
    skillLayer->setVisible(false);
    equipmentLayer->setVisible(true);
}

// �������������ס���Ʒ���ӵ���ʾ
void BackpackScene::createEquipmentGrid() {
    float startX = 50;
    float startY = Director::getInstance()->getVisibleSize().height - 100; // ���ӵ���ʼλ��

    // ������������
    auto weaponBox = Sprite::create("UI/item_slot.png");
    weaponBox->setPosition(Vec2(startX, startY));
    this->addChild(weaponBox);

    // �������׸���
    auto armorBox = Sprite::create("UI/item_slot.png");
    armorBox->setPosition(Vec2(startX, startY - 80));
    this->addChild(armorBox);

    // ������Ʒ����
    auto accessoryBox = Sprite::create("UI/item_slot.png");
    accessoryBox->setPosition(Vec2(startX, startY - 160));
    this->addChild(accessoryBox);
}

// �������������ӵ���ʾ
void BackpackScene::createSkillGrid() {
    float startX = 200;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    for (int i = 0; i < 4; i++) {
        auto skillBox = Sprite::create("item_slot.png");
        skillBox->setPosition(Vec2(startX, startY - i * 80));
        this->addChild(skillBox);
    }
}

// �������������ʾ�����顢�ȼ��������ȣ�
void BackpackScene::createPlayerDataDisplay() {
    auto levelLabel = Label::createWithTTF("Level: " + std::to_string(player->getLevel()), "fonts/Marker Felt.ttf", 24);
    levelLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 150, 100));
    this->addChild(levelLabel);

    auto staminaLabel = Label::createWithTTF("Stamina: " + std::to_string(player->getStamina()), "fonts/Marker Felt.ttf", 24);
    staminaLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 150, 50));
    this->addChild(staminaLabel);
}

