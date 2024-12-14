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
    // 创建并返回主背包层对象，传入玩家数据
    auto layer = BackpackMainLayer::create();
    layer->setPlayer(player);
    return layer;
}

bool BackpackMainLayer::init() {
    if (!Layer::init()) {
        return false;
    }

   
    // 设定背包层的大小，可以根据需要做调整
    this->setContentSize(Size(960, 540));  // 使背包层大小与屏幕一致

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

    // 创建玩家形象切换按钮
    createArrowButtons();

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

void BackpackMainLayer::setPlayer(Player* player) {
    this->player = player;
    if (player) {
        // 确保玩家数据已经设置

    // 创建玩家数据显示（经验、等级、体力等）
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

// 创建武器、盔甲、饰品格子的显示
void BackpackMainLayer::createEquipmentGrid() {
    float startX = 100;
    float startY = Director::getInstance()->getVisibleSize().height - 100; // 格子的起始位置

    // 创建武器格子
    auto weaponBox = Sprite::create("UI/item_slot.png");
    weaponBox->setPosition(Vec2(startX, startY));
    this->addChild(weaponBox);

    // 创建盔甲格子
    auto armorBox = Sprite::create("UI/item_slot.png");
    armorBox->setPosition(Vec2(startX, startY - 90));
    this->addChild(armorBox);

    // 创建饰品格子
    auto accessoryBox = Sprite::create("UI/item_slot.png");
    accessoryBox->setPosition(Vec2(startX, startY - 180));
    this->addChild(accessoryBox);
}

// 创建技能栏格子的显示
void BackpackMainLayer::createSkillGrid() {
    float startX = 500;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    for (int i = 0; i < 4; i++) {
        auto skillBox = Sprite::create("UI/item_slot.png");
        skillBox->setPosition(Vec2(startX, startY - i * 90));
        this->addChild(skillBox);
    }
}

// 创建玩家数据显示（经验、等级、体力等）
void BackpackMainLayer::createPlayerDataDisplay() {
    float startX = 300;
    float startY = Director::getInstance()->getVisibleSize().height - 500; // 格子的起始位置
    auto levelLabel = Label::createWithTTF("Level: " + std::to_string(player->getLevel()), "fonts/Marker Felt.ttf", 24);
    levelLabel->setPosition(Vec2(startX, startY));
    this->addChild(levelLabel);

    auto staminaLabel = Label::createWithTTF("Stamina: " + std::to_string(player->getStamina()), "fonts/Marker Felt.ttf", 24);
    staminaLabel->setPosition(Vec2(startX, startY - 50));
    this->addChild(staminaLabel);
}

void BackpackMainLayer::updatePlayerData() {
    // 更新数据显示
}

void BackpackMainLayer::adjustSizeForTransition() {
    float scaleX = Director::getInstance()->getOpenGLView()->getFrameSize().width / 1920.0f*0.415f;
    float scaleY = Director::getInstance()->getOpenGLView()->getFrameSize().height / 1080.0f*0.415f;
    this->setScale(scaleX, scaleY);
    this->setAnchorPoint(Vec2(0.5, 0.5));  // 设置锚点为中心

}

void BackpackMainLayer::createArrowButtons() {
    // 加载整张图片到纹理
    auto texture = Director::getInstance()->getTextureCache()->addImage("UI/item_button.png");

    // 裁剪左箭头按钮
    auto leftButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(16, 0, 16, 16));
    auto leftButton = Sprite::createWithSpriteFrame(leftButtonFrame);
    leftButton->setPosition(Vec2(200, Director::getInstance()->getVisibleSize().height - 200)); // 设置左箭头的位置
    leftButton->setScale(3.0f); // 放大2倍
    this->addChild(leftButton);

    // 裁剪右箭头按钮
    auto rightButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(32, 0, 16, 16));
    auto rightButton = Sprite::createWithSpriteFrame(rightButtonFrame);
    rightButton->setPosition(Vec2(400, Director::getInstance()->getVisibleSize().height - 200)); // 设置右箭头的位置
    rightButton->setScale(3.0f); // 放大2倍
    this->addChild(rightButton);
}