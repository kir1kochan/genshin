#include "BackpackMainLayer.h"
#include "BackpackLayer.h"
#include "SkillLayer.h"
#include "EquipmentLayer.h"
#include "../../Common/Entities/Player/Player.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

BackpackMainLayer::BackpackMainLayer()
    : player(nullptr), backpackLayer(nullptr), skillLayer(nullptr), equipmentLayer(nullptr),
    nextButton(nullptr), prevButton(nullptr) {}

BackpackMainLayer::~BackpackMainLayer() {}

BackpackMainLayer* BackpackMainLayer::create(Player* player) {
    // ��������������������󣬴����������
    auto layer = BackpackMainLayer::create();
    layer->setPlayer(player);
    return layer;
}

bool BackpackMainLayer::init() {
    if (!Layer::init()) {
        return false;
    }

   
    // �趨������Ĵ�С�����Ը�����Ҫ������
    this->setContentSize(Size(960, 540));  // ʹ�������С����Ļһ��

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

    // ������������л���ť
    createArrowButtons();

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

void BackpackMainLayer::setPlayer(Player* player) {
    this->player = player;
    if (player) {
        // ȷ����������Ѿ�����

    // �������������ʾ�����顢�ȼ��������ȣ�
        createPlayerDataDisplay();
    }
}

Player* BackpackMainLayer::getPlayer() const {
    return player;
}

void BackpackMainLayer::switchToBackpackLayer() {
    backpackLayer->setVisible(true);
    skillLayer->setVisible(false);
    equipmentLayer->setVisible(false);
}

void BackpackMainLayer::switchToSkillLayer() {
    backpackLayer->setVisible(false);
    skillLayer->setVisible(true);
    equipmentLayer->setVisible(false);
}

void BackpackMainLayer::switchToEquipmentLayer() {
    backpackLayer->setVisible(false);
    skillLayer->setVisible(false);
    equipmentLayer->setVisible(true);
}

// �������������ס���Ʒ���ӵ���ʾ
void BackpackMainLayer::createEquipmentGrid() {
    float startX = 100;
    float startY = Director::getInstance()->getVisibleSize().height - 100; // ���ӵ���ʼλ��

    // ������������
    auto weaponBox = Sprite::create("UI/item_slot.png");
    weaponBox->setPosition(Vec2(startX, startY));
    this->addChild(weaponBox);

    // �������׸���
    auto armorBox = Sprite::create("UI/item_slot.png");
    armorBox->setPosition(Vec2(startX, startY - 90));
    this->addChild(armorBox);

    // ������Ʒ����
    auto accessoryBox = Sprite::create("UI/item_slot.png");
    accessoryBox->setPosition(Vec2(startX, startY - 180));
    this->addChild(accessoryBox);
}

// �������������ӵ���ʾ
void BackpackMainLayer::createSkillGrid() {
    float startX = 500;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    for (int i = 0; i < 4; i++) {
        auto skillBox = Sprite::create("UI/item_slot.png");
        skillBox->setPosition(Vec2(startX, startY - i * 90));
        this->addChild(skillBox);
    }
}

// �������������ʾ�����顢�ȼ��������ȣ�
void BackpackMainLayer::createPlayerDataDisplay() {
    float startX = 300;
    float startY = Director::getInstance()->getVisibleSize().height - 500; // ���ӵ���ʼλ��
    auto levelLabel = Label::createWithTTF("Level: " + std::to_string(player->getLevel()), "fonts/Marker Felt.ttf", 24);
    levelLabel->setPosition(Vec2(startX, startY));
    this->addChild(levelLabel);

    auto staminaLabel = Label::createWithTTF("Stamina: " + std::to_string(player->getStamina()), "fonts/Marker Felt.ttf", 24);
    staminaLabel->setPosition(Vec2(startX, startY - 50));
    this->addChild(staminaLabel);
}

void BackpackMainLayer::updatePlayerData() {
    // ����������ʾ
}

void BackpackMainLayer::adjustSizeForTransition() {
    float scaleX = Director::getInstance()->getOpenGLView()->getFrameSize().width / 1920.0f*0.415f;
    float scaleY = Director::getInstance()->getOpenGLView()->getFrameSize().height / 1080.0f*0.415f;
    this->setScale(scaleX, scaleY);
    this->setAnchorPoint(Vec2(0.5, 0.5));  // ����ê��Ϊ����

}

void BackpackMainLayer::createArrowButtons() {
    // ��������ͼƬ������
    auto texture = Director::getInstance()->getTextureCache()->addImage("UI/item_button.png");

    // �ü����ͷ��ť
    auto leftButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(16, 0, 16, 16));
    auto leftButton = Sprite::createWithSpriteFrame(leftButtonFrame);
    leftButton->setPosition(Vec2(200, Director::getInstance()->getVisibleSize().height - 200)); // �������ͷ��λ��
    leftButton->setScale(3.0f); // �Ŵ�2��
    this->addChild(leftButton);

    // �ü��Ҽ�ͷ��ť
    auto rightButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(32, 0, 16, 16));
    auto rightButton = Sprite::createWithSpriteFrame(rightButtonFrame);
    rightButton->setPosition(Vec2(400, Director::getInstance()->getVisibleSize().height - 200)); // �����Ҽ�ͷ��λ��
    rightButton->setScale(3.0f); // �Ŵ�2��
    this->addChild(rightButton);
}