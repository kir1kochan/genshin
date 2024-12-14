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
    auto background = LayerColor::create(Color4B(139, 69, 19, 255)); // 棕色背景,可以修改成别的背景
    this->addChild(background);

    // 初始化装备图标的容器节点
    equipmentIconsContainer = Node::create();
    this->addChild(equipmentIconsContainer);

    // 初始化玩家信息的容器节点
    playerInfoContainer = Node::create();
    this->addChild(playerInfoContainer);

    // 初始化技能图标的容器节点
    skillIconsContainer = Node::create();
    this->addChild(skillIconsContainer);
    
    // 创建并初始化背包界面
    backpackLayer = BackpackLayer::create();
    this->addChild(backpackLayer);
    
    // 创建并初始化技能界面
    skillLayer = SkillLayer::create();
    this->addChild(skillLayer);

    /*
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
    playerInfoContainer->removeAllChildren();

    auto levelLabel = Label::createWithSystemFont("Level: " + std::to_string((int)player->getLevel()), "fonts/Marker Felt.ttf", 24);
    levelLabel->setPosition(Vec2(startX, startY));
    playerInfoContainer->addChild(levelLabel);

    auto expLabel = Label::createWithSystemFont("Exp: " + std::to_string((int)player->getExp()) + "/" + std::to_string((int)player->getExperienceForNextLevel()), "fonts/Marker Felt.ttf", 24);
    expLabel->setPosition(Vec2(startX, startY - 50));
    playerInfoContainer->addChild(expLabel);

    auto healthLabel = Label::createWithSystemFont("Health: " + std::to_string((int)player->getHealth())+"/"+std::to_string((int)player->getMaxHealth()), "fonts/Marker Felt.ttf", 24);
    healthLabel->setPosition(Vec2(startX, startY - 100));
    playerInfoContainer->addChild(healthLabel);

    auto staminaLabel = Label::createWithSystemFont("Stamina: " + std::to_string((int)player->getStamina()) + "/" + std::to_string((int)player->getMaxStamina()), "fonts/Marker Felt.ttf", 24);
    staminaLabel->setPosition(Vec2(startX, startY - 150));
    playerInfoContainer->addChild(staminaLabel);

    auto attackLabel = Label::createWithSystemFont("Attack: " + std::to_string((int)player->getAttack()), "fonts/Marker Felt.ttf", 24);
    attackLabel->setPosition(Vec2(startX, startY - 200));
    playerInfoContainer->addChild(attackLabel);

    auto elementLabel = Label::createWithSystemFont("Element: " + elementToString(player->getElement()), "fonts/Marker Felt.ttf", 24);
    elementLabel->setPosition(Vec2(startX, startY - 300));
    playerInfoContainer->addChild(elementLabel);
    std::shared_ptr<Armor> armor = player->getArmor();
    int defence;
    if (armor) {
        defence = (int)player->getArmor().get()->getPower();
    }
    else
        defence = 0;
    auto defenceLabel = Label::createWithSystemFont("Defence: " + std::to_string(defence), "fonts/Marker Felt.ttf", 24);
    defenceLabel->setPosition(Vec2(startX, startY - 250));
    playerInfoContainer->addChild(defenceLabel);
}

void BackpackMainLayer::updatePlayerData() {
    // 更新数据显示
    createPlayerDataDisplay();
}

void BackpackMainLayer::adjustSizeForTransition() {
    float scaleX = Director::getInstance()->getOpenGLView()->getFrameSize().width / 1920.0f*0.415f;
    float scaleY = Director::getInstance()->getOpenGLView()->getFrameSize().height / 1080.0f*0.415f;
    this->setScale(scaleX, scaleY);
    this->setAnchorPoint(Vec2(0.5, 0.5));  // 设置锚点为中心
    updatePlayerData();
    refreshEquipmentIcons();
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

void BackpackMainLayer::createEquipmentIcons() {
    if (!player) {
        return; // 确保 player 不为空
    }

    // 清空装备图标容器
    equipmentIconsContainer->removeAllChildren();

    // 获取玩家的装备
    std::shared_ptr<Weapon> weapon = player->getWeapon();
    std::shared_ptr<Armor> armor = player->getArmor();
    std::shared_ptr<Accessory> accessory = player->getAccessory();

    // 设置图标显示的起始位置
    float startX = 100;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    // 生成武器图标
    if (weapon) {
        std::string weaponFilePath = "Equipment/" + weapon->getName() + ".jpg";
        auto weaponIcon = Sprite::create(weaponFilePath);
        weaponIcon->setPosition(Vec2(startX, startY));
        equipmentIconsContainer->addChild(weaponIcon);
        // 显示包围盒
        createBoundingBoxForIcons(weaponIcon);

        // 延迟添加双击卸下的监听器
        scheduleOnce([this, weaponIcon](float) {
            if (weaponIcon) {  // 确保 weaponIcon 仍然有效
                addDoubleClickListener(weaponIcon, [this]() {
                    player->unequipWeapon();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "weaponListener");
    }

    // 生成盔甲图标
    if (armor) {
        std::string armorFilePath = "Equipment/" + armor->getName() + ".jpg";
        auto armorIcon = Sprite::create(armorFilePath);
        armorIcon->setPosition(Vec2(startX, startY - 90));
        equipmentIconsContainer->addChild(armorIcon);

        // 延迟添加双击卸下的监听器
        scheduleOnce([this, armorIcon](float) {
            if (armorIcon) {  // 确保 armorIcon 仍然有效
                addDoubleClickListener(armorIcon, [this]() {
                    player->unequipArmor();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "armorListener");
    }

    // 生成饰品图标
    if (accessory) {
        std::string accessoryFilePath = "Equipment/" + accessory->getName() + ".jpg";
        auto accessoryIcon = Sprite::create(accessoryFilePath);
        accessoryIcon->setPosition(Vec2(startX, startY - 180));
        equipmentIconsContainer->addChild(accessoryIcon);

        // 延迟添加双击卸下的监听器
        scheduleOnce([this, accessoryIcon](float) {
            if (accessoryIcon) {  // 确保 accessoryIcon 仍然有效
                addDoubleClickListener(accessoryIcon, [this]() {
                    player->unequipAccessory();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "accessoryListener");
    }
}

void BackpackMainLayer::refreshEquipmentIcons() {
    createEquipmentIcons();
}

void BackpackMainLayer::addDoubleClickListener(cocos2d::Sprite* target, const std::function<void()>& callback) {
    static const float DOUBLE_CLICK_INTERVAL = 0.3f; // 双击的时间间隔（秒）
    static float lastClickTime = 0.0f; // 上一次点击时间

    auto listener = EventListenerMouse::create();

    listener->onMouseDown = [target, callback](EventMouse* event) {
        // 获取鼠标的位置（世界坐标）
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        Vec2 locationInNode = cursorPosition;
        Rect rect = target->getBoundingBox();

        if ((locationInNode.x <= rect.origin.x && locationInNode.x >= rect.origin.x - rect.size.width) &&
            (locationInNode.y >= rect.origin.y && locationInNode.y <= rect.origin.y + rect.size.height)) {
            float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;

            if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                callback(); // 双击触发回调
                lastClickTime = 0.0f;
            }
            else {
                lastClickTime = currentTime; // 记录时间
            }
        }
        };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
}



void BackpackMainLayer::createBoundingBoxForIcons(cocos2d::Sprite* sprite) {
    if (!sprite) return;

    // 获取 BoundingBox
    cocos2d::Rect box = sprite->getBoundingBox();

    // 创建一个 DrawNode 绘制包围盒
    auto drawNode = cocos2d::DrawNode::create();
    drawNode->drawRect(cocos2d::Vec2(box.origin.x, box.origin.y),
        cocos2d::Vec2(box.origin.x + box.size.width, box.origin.y + box.size.height),
        cocos2d::Color4F::RED);

    // 添加到父节点
    this->addChild(drawNode);
}