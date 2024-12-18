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

BackpackMainLayer::BackpackMainLayer(Player* player)
    : player(player), backpackLayer(nullptr), skillLayer(nullptr), equipmentLayer(nullptr),
    nextButton(nullptr), prevButton(nullptr) {}

BackpackMainLayer::~BackpackMainLayer() {}

BackpackMainLayer* BackpackMainLayer::create(Player* player) {
    // 创建并返回主背包层对象，传入玩家数据
    auto layer = new (std::nothrow) BackpackMainLayer(player);  // 使用带参数的构造函数
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
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

    // 创建背包界面的UI组件
    createBackpackUI();

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
    skillLayer->setPlayer(player);
    skillLayer->setName("skillLayer");
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

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // 每帧调用 update 方法
        skillLayer->updateSkills();
        }, 0.5f, "update_key");

    // 增加悬停监听器
    addHoverListener();
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
    weaponBox->setScale(1.3f);
    this->addChild(weaponBox);

    // 创建盔甲格子
    auto armorBox = Sprite::create("UI/item_slot.png");
    armorBox->setPosition(Vec2(startX, startY - 110));
    armorBox->setScale(1.3f);
    this->addChild(armorBox);

    // 创建饰品格子
    auto accessoryBox = Sprite::create("UI/item_slot.png");
    accessoryBox->setPosition(Vec2(startX, startY - 220));
    accessoryBox->setScale(1.3f);
    this->addChild(accessoryBox);
}

// 创建技能栏格子的显示
void BackpackMainLayer::createSkillGrid() {
    float startX = 500;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    for (int i = 0; i < 4; i++) {
        auto skillBox = Sprite::create("UI/item_slot.png");
        skillBox->setPosition(Vec2(startX, startY - i * 110));
        skillBox->setScale(1.3f);
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
    skillLayer->update();
}

void BackpackMainLayer::createArrowButtons() {
    // 加载整张图片到纹理
    auto texture = Director::getInstance()->getTextureCache()->addImage("UI/item_button.png");

    // 裁剪左箭头按钮
    auto leftButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(16, 0, 16, 16));
    auto leftButton = Sprite::createWithSpriteFrame(leftButtonFrame);
    leftButton->setPosition(Vec2(200, Director::getInstance()->getVisibleSize().height - 200)); // 设置左箭头的位置
    leftButton->setScale(3.0f); // 放大2倍
    leftButton->setName("leftButton");
    this->addChild(leftButton);

    // 裁剪右箭头按钮
    auto rightButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(32, 0, 16, 16));
    auto rightButton = Sprite::createWithSpriteFrame(rightButtonFrame);
    rightButton->setPosition(Vec2(400, Director::getInstance()->getVisibleSize().height - 200)); // 设置右箭头的位置
    rightButton->setScale(3.0f); // 放大2倍
    rightButton->setName("rightButton");
    this->addChild(rightButton);

    // 添加鼠标监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [this](Event* event) {
        EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
        if (!mouseEvent) return;

        Vec2 clickPosition = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        
        auto leftButton = this->getChildByName("leftButton");
        auto rightButton = this->getChildByName("rightButton");
        Rect boundingBox = leftButton->getBoundingBox();
        Vec2 pos = leftButton->getPosition();

        // 调整boundingBox的偏移
        boundingBox.origin = pos - Vec2(boundingBox.size.width, boundingBox.size.height/2);
        if (boundingBox.containsPoint(clickPosition)) {
            this->onLeftArrowClicked();
        }
        else if (rightButton->getBoundingBox().containsPoint(clickPosition)) {
            this->onRightArrowClicked();
        }
    };

    // 添加监听器到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
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
        weaponIcon->setScale(1.3f);
        equipmentIconsContainer->addChild(weaponIcon);
        // 显示包围盒
        createBoundingBoxForIcons(weaponIcon);
        addHoverListenerForIcons(weaponIcon, weapon.get()->getName(), std::to_string(weapon.get()->getPower()), weapon.get()->getId(), [this, weaponIcon]() { player->unequipWeapon();
        this->eraseHoverListenerForIcons(weaponIcon); this->refreshEquipmentIcons(); });
        /* 延迟添加双击卸下的监听器和悬停监听
        scheduleOnce([this, weaponIcon,weapon](float) {
            if (weaponIcon) {  // 确保 weaponIcon 仍然有效
                addDoubleClickListener(weaponIcon, [this]() {
                    player->unequipWeapon();
                    this->refreshEquipmentIcons();
                    });
                
            }
            }, 1.0f, "weaponListener");*/
    }

    // 生成盔甲图标
    if (armor) {
        std::string armorFilePath = "Equipment/" + armor->getName() + ".jpg";
        auto armorIcon = Sprite::create(armorFilePath);
        armorIcon->setPosition(Vec2(startX, startY - 110));
        armorIcon->setScale(1.3f);
        equipmentIconsContainer->addChild(armorIcon);
        addHoverListenerForIcons(armorIcon, armor.get()->getName(), std::to_string(armor.get()->getPower()), armor.get()->getId(), [this,armorIcon]() { player->unequipArmor(); 
        this->eraseHoverListenerForIcons(armorIcon); this->refreshEquipmentIcons(); });
        // 延迟添加双击卸下的监听器
        /*scheduleOnce([this, armorIcon](float) {
            if (armorIcon) {  // 确保 armorIcon 仍然有效
                addDoubleClickListener(armorIcon, [this]() {
                    player->unequipArmor();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "armorListener");
    */
    }

    // 生成饰品图标
    if (accessory) {
        std::string accessoryFilePath = "Equipment/" + accessory->getName() + ".jpg";
        auto accessoryIcon = Sprite::create(accessoryFilePath);
        accessoryIcon->setPosition(Vec2(startX, startY - 220));
        accessoryIcon->setScale(1.3f);
        equipmentIconsContainer->addChild(accessoryIcon);
        addHoverListenerForIcons(accessoryIcon, accessory.get()->getName(), std::to_string(accessory.get()->getPower()), accessory.get()->getId(), [this, accessoryIcon]() { player->unequipAccessory();
        this->eraseHoverListenerForIcons(accessoryIcon); this->refreshEquipmentIcons(); });
        /* 延迟添加双击卸下的监听器
        scheduleOnce([this, accessoryIcon](float) {
            if (accessoryIcon) {  // 确保 accessoryIcon 仍然有效
                addDoubleClickListener(accessoryIcon, [this]() {
                    player->unequipAccessory();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "accessoryListener");*/
    }
}

void BackpackMainLayer::refreshEquipmentIcons() {
    createEquipmentIcons();
}

/*void BackpackMainLayer::addDoubleClickListener(cocos2d::Sprite* target, const std::function<void()>& callback) {
    

    auto listener = EventListenerMouse::create();

    

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
}
*/


// 调试用包围盒绘制
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

void BackpackMainLayer::onLeftArrowClicked() {
    // 左箭头点击逻辑，切换玩家属性
    int element = static_cast<int>(player->getElement());
    element = (element + 6) % 7;
    player->chanegElement(static_cast<Element>(element));
    createPlayerDataDisplay();
    // 还需要添加玩家形象变化
    CCLOG("Left arrow action triggered!");
}

void BackpackMainLayer::onRightArrowClicked() {
    // 右箭头点击逻辑，切换玩家属性
    int element = static_cast<int>(player->getElement());
    element = (element + 8) % 7;
    player->chanegElement(static_cast<Element>(element));
    createPlayerDataDisplay();
    // 还需要添加玩家形象变化
    CCLOG("Right arrow action triggered!");
}

void BackpackMainLayer::createBackpackUI() {
    // 创建一个标签显示背包界面的标题
    auto label = Label::createWithSystemFont("Backpack", "Arial", 24);
    label->setPosition(Vec2(1200, 1050));
    this->addChild(label);

    // 物品格子的起始位置
    float startX = 700.0f;
    float startY = 960.0f;

    // 每行显示多少个格子
    int itemsPerRow = 9;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;

    // 创建并平铺格子
    for (int i = 0; i < 63; ++i) { 
        // 计算当前格子的位置
        float x = startX + (i % itemsPerRow) * (itemWidth + 25); // 每个格子之间有 5 的间隔
        float y = startY - (i / itemsPerRow) * (itemHeight + 25); // 每行的格子间隔 5

        // 创建格子的 Sprite 并设置位置
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        itemSprite->setScale(1.3f);
        this->addChild(itemSprite);
    }
}

// 添加到鼠标悬停监听队列
void BackpackMainLayer::addHoverListenerForIcons(Sprite* icon, const std::string& name, const std::string& effectValue, int id, std::function<void()> cb) {

    // 显示物品的名称和效果
    int type = id / 100000;
    int subType = id % 1000 / 100;
    std::string effect;
    // 获取图标的边界
    Rect boundingBox = icon->getBoundingBox();
    // 调整boundingBox的偏移
    if (type != 9) {
        boundingBox.origin = boundingBox.origin - Vec2(boundingBox.size.width, 0);
    }
    else {
        boundingBox.origin = boundingBox.origin - Vec2(boundingBox.size.width/2, 0);
    }
    switch (type) {
    case 1: // 装备类
        switch (subType) {
        case 1: {
            effect = "Attack: " + effectValue;
            break;
        }
        case 2: {
            effect = "Defence: " + effectValue;
            break;
        }
        case 3: {
            effect = effectValue + " Stamina  regen per second";
            break;
        }
        default:
            CCLOG("Error: Unsupported equipment subtype %d for ID %d", subType, id);
            break;
        }
        break;
    case 2: // 药剂类
        effect = effectValue + " Stamina  regen";
        break;
    case 3: // 食物类
        effect = effectValue + " Health  regen";
        break;
    case 9: // 技能
        effect = "StaminaCost: " + effectValue;
        break;
    default:
        CCLOG("Error: Unsupported type %d for ID %d", type, id);
        break;
    }
    // 根据物品类型生成效果信息
    HoverInfo hoverInfo;
    hoverInfo.name = name;
    hoverInfo.effectValue = effect;
    hoverInfo.callback = cb;
    // 根据 Rect 的位置划分到对应区域
    if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() > 490) {
        // 左上区域
        topLeftGrid[boundingBox] = hoverInfo;
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() > 490) {
        // 右上区域
        topRightGrid[boundingBox] = hoverInfo;
    }
    else if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() <= 490) {
        // 左下区域
        bottomLeftGrid[boundingBox] = hoverInfo;
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() <= 490) {
        // 右下区域
        bottomRightGrid[boundingBox] = hoverInfo;
    }
}

// 移出鼠标悬停监听队列
void BackpackMainLayer::eraseHoverListenerForIcons(Sprite* icon) {

    // 获取图标的边界
    Rect boundingBox = icon->getBoundingBox();
    // 调整boundingBox的偏移
    boundingBox.origin = boundingBox.origin - Vec2(boundingBox.size.width, 0);
    if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() > 490) {
        // 左上区域
        topLeftGrid.erase(boundingBox);
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() > 490) {
        // 右上区域
        topRightGrid.erase(boundingBox);
    }
    else if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() <= 490) {
        // 左下区域
        bottomLeftGrid.erase(boundingBox);
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() <= 490) {
        // 右下区域
        bottomRightGrid.erase(boundingBox);
    }
}

// 主背包层部分的技能信息显示
void BackpackMainLayer::showHoverInfo(const std::string& name, const std::string& effectValue, const Vec2& position) {
    // 创建一个背景 LayerColor
    if (!_hoverLabelBackground) {
        _hoverLabelBackground = LayerColor::create(Color4B(0, 0, 0, 180));  // 黑色半透明背景
        _hoverLabelBackground->setContentSize(Size(200, 100));  // 设置背景的大小，可以根据需要调整
        this->addChild(_hoverLabelBackground);
    }

    // 创建一个 Label 显示悬停信息
    if (!_hoverLabel) {
        _hoverLabel = Label::createWithSystemFont("", "Arial", 24);
        _hoverLabel->setColor(Color3B::WHITE);
        _hoverLabelBackground->addChild(_hoverLabel);  // 将 Label 添加到背景上
    }
    _hoverLabelBackground->setVisible(true);
    // 更新 Label 的内容
    std::string hoverText = name + "\n" + effectValue;
    _hoverLabel->setString(hoverText);

    // 更新背景大小
    _hoverLabelBackground->setContentSize(Size(_hoverLabel->getContentSize().width + 20, _hoverLabel->getContentSize().height + 20));

    // 更新背景和文本的位置
    _hoverLabelBackground->setPosition(position + Vec2(50, 0)); // 背景位置
    _hoverLabel->setPosition(Vec2(_hoverLabelBackground->getContentSize().width / 2, _hoverLabelBackground->getContentSize().height / 2)); // 文字居中
}

// 隐藏悬停信息
void BackpackMainLayer::hideHoverInfo() {
    if (mouseArea==0) {
        // 左上区域
        for (const auto entry : topLeftGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    else if (mouseArea == 1) {        
        // 右上区域
        for (const auto& entry : topRightGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    else if (mouseArea = 2) {
        // 左下区域
        for (const auto& entry : bottomLeftGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    else if (mouseArea == 3) {       
        // 右下区域
        for (const auto& entry : topRightGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    if (_hoverLabel) {
        _hoverLabel->setString("");  // 清空文本
    }
    if (_hoverLabelBackground) {
        _hoverLabelBackground->setVisible(false);  // 隐藏背景
    }
}

void BackpackMainLayer::update(float deltatime) {
    if (gaptime < 2.0f) {
        gaptime += deltatime;
        return;
    }
    hideHoverInfo();
    gaptime = 0;
}

void BackpackMainLayer::addHoverListener() {
    static const float DOUBLE_CLICK_INTERVAL = 0.2f; // 双击的时间间隔（秒）
    static float lastClickTime = 0.0f; // 上一次点击时间

    auto listener = EventListenerMouse::create();

    listener->onMouseMove = [this](EventMouse* event) {
        // 获取鼠标位置
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        // 根据 Rect 的位置划分到对应区域
        if (cursorPosition.x <= 1160 && cursorPosition.y > 490) {
            mouseArea = 0;
            // 左上区域
            for (auto entry : topLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    entry.second.isHovering = true;  // 更新当前悬停信息
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y > 490) {
            mouseArea = 1;
            // 右上区域
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    entry.second.isHovering = true;  // 更新当前悬停信息
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }
        else if (cursorPosition.x <= 1160 && cursorPosition.y <= 490) {
            mouseArea = 2;
            // 左下区域
            for (auto& entry : bottomLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    entry.second.isHovering = true;  // 更新当前悬停信息
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y <= 490) {
            mouseArea = 3;
            // 右下区域
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    entry.second.isHovering = true;  // 更新当前悬停信息
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }

        };

    listener->onMouseDown = [this](EventMouse* event) {
        // 获取鼠标位置
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        // 根据 Rect 的位置划分到对应区域
        if (cursorPosition.x <= 1160 && cursorPosition.y > 490) {
            // 左上区域
            for (auto entry : topLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // 双击触发回调
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // 记录时间
                    }
                    return;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y > 490) {
            // 右上区域
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // 双击触发回调
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // 记录时间
                    }
                    return;
                }
            }
        }
        else if (cursorPosition.x <= 1160 && cursorPosition.y <= 490) {
            // 左下区域
            for (auto& entry : bottomLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // 双击触发回调
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // 记录时间
                    }
                    return;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y <= 490) {
            // 右下区域
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // 双击触发回调
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // 记录时间
                    }
                    return;
                }
            }
        }

        };
    // 注册监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}