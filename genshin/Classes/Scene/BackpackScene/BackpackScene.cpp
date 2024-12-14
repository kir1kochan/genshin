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
    // 创建并返回主场景对象，传入玩家数据
    auto scene = BackpackScene::create();
    scene->setPlayer(player);
    return scene;
}

bool BackpackScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 背景颜色设置为棕色
    auto background = LayerColor::create(Color4B(139, 69, 19, 255)); // 棕色背景
    this->addChild(background);

    /*
    // 创建并初始化背包界面
    backpackLayer = BackpackLayer::create();
    this->addChild(backpackLayer);

    // 创建并初始化技能界面
    skillLayer = SkillLayer::create();
    this->addChild(skillLayer);

    // 创建并初始化装备界面
    equipmentLayer = EquipmentLayer::create();
    this->addChild(equipmentLayer);

    // 初始设置为显示背包界面
    switchToBackpackLayer();
    */

    // 创建固定的武器、盔甲、饰品格子
    createEquipmentGrid();

    // 创建技能栏格子
    createSkillGrid();

    // 创建玩家数据显示（经验、等级、体力等）
    createPlayerDataDisplay();

    /*
    // 创建并初始化切换到下一个界面按钮
    nextButton = Button::create("next_button.png");
    nextButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 50, Director::getInstance()->getVisibleSize().height / 2));
    // 设置切换到下一个界面的点击事件
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

    // 创建并初始化切换到上一个界面按钮
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

// 创建武器、盔甲、饰品格子的显示
void BackpackScene::createEquipmentGrid() {
    float startX = 50;
    float startY = Director::getInstance()->getVisibleSize().height - 100; // 格子的起始位置

    // 创建武器格子
    auto weaponBox = Sprite::create("UI/item_slot.png");
    weaponBox->setPosition(Vec2(startX, startY));
    this->addChild(weaponBox);

    // 创建盔甲格子
    auto armorBox = Sprite::create("UI/item_slot.png");
    armorBox->setPosition(Vec2(startX, startY - 80));
    this->addChild(armorBox);

    // 创建饰品格子
    auto accessoryBox = Sprite::create("UI/item_slot.png");
    accessoryBox->setPosition(Vec2(startX, startY - 160));
    this->addChild(accessoryBox);
}

// 创建技能栏格子的显示
void BackpackScene::createSkillGrid() {
    float startX = 200;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    for (int i = 0; i < 4; i++) {
        auto skillBox = Sprite::create("item_slot.png");
        skillBox->setPosition(Vec2(startX, startY - i * 80));
        this->addChild(skillBox);
    }
}

// 创建玩家数据显示（经验、等级、体力等）
void BackpackScene::createPlayerDataDisplay() {
    auto levelLabel = Label::createWithTTF("Level: " + std::to_string(player->getLevel()), "fonts/Marker Felt.ttf", 24);
    levelLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 150, 100));
    this->addChild(levelLabel);

    auto staminaLabel = Label::createWithTTF("Stamina: " + std::to_string(player->getStamina()), "fonts/Marker Felt.ttf", 24);
    staminaLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 150, 50));
    this->addChild(staminaLabel);
}

