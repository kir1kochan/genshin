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
    // ��������������������󣬴����������
    auto layer = new (std::nothrow) BackpackMainLayer(player);  // ʹ�ô������Ĺ��캯��
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

   
    // �趨������Ĵ�С�����Ը�����Ҫ������
    this->setContentSize(Size(960, 540));  // ʹ�������С����Ļһ��

    // ������ɫ����Ϊ��ɫ
    auto background = LayerColor::create(Color4B(139, 69, 19, 255)); // ��ɫ����,�����޸ĳɱ�ı���
    this->addChild(background);

    // �������������UI���
    createBackpackUI();

    // ��ʼ��װ��ͼ��������ڵ�
    equipmentIconsContainer = Node::create();
    this->addChild(equipmentIconsContainer);

    // ��ʼ�������Ϣ�������ڵ�
    playerInfoContainer = Node::create();
    this->addChild(playerInfoContainer);

    // ��ʼ������ͼ��������ڵ�
    skillIconsContainer = Node::create();
    this->addChild(skillIconsContainer);
    
    // ��������ʼ����������
    backpackLayer = BackpackLayer::create();
    this->addChild(backpackLayer);
    
    // ��������ʼ�����ܽ���
    skillLayer = SkillLayer::create();
    skillLayer->setPlayer(player);
    skillLayer->setName("skillLayer");
    this->addChild(skillLayer);

    /*
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

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // ÿ֡���� update ����
        skillLayer->updateSkills();
        }, 0.5f, "update_key");

    // ������ͣ������
    addHoverListener();
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
    weaponBox->setScale(1.3f);
    this->addChild(weaponBox);

    // �������׸���
    auto armorBox = Sprite::create("UI/item_slot.png");
    armorBox->setPosition(Vec2(startX, startY - 110));
    armorBox->setScale(1.3f);
    this->addChild(armorBox);

    // ������Ʒ����
    auto accessoryBox = Sprite::create("UI/item_slot.png");
    accessoryBox->setPosition(Vec2(startX, startY - 220));
    accessoryBox->setScale(1.3f);
    this->addChild(accessoryBox);
}

// �������������ӵ���ʾ
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

// �������������ʾ�����顢�ȼ��������ȣ�
void BackpackMainLayer::createPlayerDataDisplay() {
    float startX = 300;
    float startY = Director::getInstance()->getVisibleSize().height - 500; // ���ӵ���ʼλ��
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
    // ����������ʾ
    createPlayerDataDisplay();
}

void BackpackMainLayer::adjustSizeForTransition() {
    float scaleX = Director::getInstance()->getOpenGLView()->getFrameSize().width / 1920.0f*0.415f;
    float scaleY = Director::getInstance()->getOpenGLView()->getFrameSize().height / 1080.0f*0.415f;
    this->setScale(scaleX, scaleY);
    this->setAnchorPoint(Vec2(0.5, 0.5));  // ����ê��Ϊ����
    updatePlayerData();
    refreshEquipmentIcons();
    skillLayer->update();
}

void BackpackMainLayer::createArrowButtons() {
    // ��������ͼƬ������
    auto texture = Director::getInstance()->getTextureCache()->addImage("UI/item_button.png");

    // �ü����ͷ��ť
    auto leftButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(16, 0, 16, 16));
    auto leftButton = Sprite::createWithSpriteFrame(leftButtonFrame);
    leftButton->setPosition(Vec2(200, Director::getInstance()->getVisibleSize().height - 200)); // �������ͷ��λ��
    leftButton->setScale(3.0f); // �Ŵ�2��
    leftButton->setName("leftButton");
    this->addChild(leftButton);

    // �ü��Ҽ�ͷ��ť
    auto rightButtonFrame = SpriteFrame::create("UI/item_button.png", Rect(32, 0, 16, 16));
    auto rightButton = Sprite::createWithSpriteFrame(rightButtonFrame);
    rightButton->setPosition(Vec2(400, Director::getInstance()->getVisibleSize().height - 200)); // �����Ҽ�ͷ��λ��
    rightButton->setScale(3.0f); // �Ŵ�2��
    rightButton->setName("rightButton");
    this->addChild(rightButton);

    // �����������
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [this](Event* event) {
        EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
        if (!mouseEvent) return;

        Vec2 clickPosition = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        
        auto leftButton = this->getChildByName("leftButton");
        auto rightButton = this->getChildByName("rightButton");
        Rect boundingBox = leftButton->getBoundingBox();
        Vec2 pos = leftButton->getPosition();

        // ����boundingBox��ƫ��
        boundingBox.origin = pos - Vec2(boundingBox.size.width, boundingBox.size.height/2);
        if (boundingBox.containsPoint(clickPosition)) {
            this->onLeftArrowClicked();
        }
        else if (rightButton->getBoundingBox().containsPoint(clickPosition)) {
            this->onRightArrowClicked();
        }
    };

    // ��Ӽ��������¼��ַ���
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}


void BackpackMainLayer::createEquipmentIcons() {
    if (!player) {
        return; // ȷ�� player ��Ϊ��
    }

    // ���װ��ͼ������
    equipmentIconsContainer->removeAllChildren();

    // ��ȡ��ҵ�װ��
    std::shared_ptr<Weapon> weapon = player->getWeapon();
    std::shared_ptr<Armor> armor = player->getArmor();
    std::shared_ptr<Accessory> accessory = player->getAccessory();

    // ����ͼ����ʾ����ʼλ��
    float startX = 100;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    // ��������ͼ��
    if (weapon) {
        std::string weaponFilePath = "Equipment/" + weapon->getName() + ".jpg";
        auto weaponIcon = Sprite::create(weaponFilePath);
        weaponIcon->setPosition(Vec2(startX, startY));
        weaponIcon->setScale(1.3f);
        equipmentIconsContainer->addChild(weaponIcon);
        // ��ʾ��Χ��
        createBoundingBoxForIcons(weaponIcon);
        addHoverListenerForIcons(weaponIcon, weapon.get()->getName(), std::to_string(weapon.get()->getPower()), weapon.get()->getId(), [this, weaponIcon]() { player->unequipWeapon();
        this->eraseHoverListenerForIcons(weaponIcon); this->refreshEquipmentIcons(); });
        /* �ӳ����˫��ж�µļ���������ͣ����
        scheduleOnce([this, weaponIcon,weapon](float) {
            if (weaponIcon) {  // ȷ�� weaponIcon ��Ȼ��Ч
                addDoubleClickListener(weaponIcon, [this]() {
                    player->unequipWeapon();
                    this->refreshEquipmentIcons();
                    });
                
            }
            }, 1.0f, "weaponListener");*/
    }

    // ���ɿ���ͼ��
    if (armor) {
        std::string armorFilePath = "Equipment/" + armor->getName() + ".jpg";
        auto armorIcon = Sprite::create(armorFilePath);
        armorIcon->setPosition(Vec2(startX, startY - 110));
        armorIcon->setScale(1.3f);
        equipmentIconsContainer->addChild(armorIcon);
        addHoverListenerForIcons(armorIcon, armor.get()->getName(), std::to_string(armor.get()->getPower()), armor.get()->getId(), [this,armorIcon]() { player->unequipArmor(); 
        this->eraseHoverListenerForIcons(armorIcon); this->refreshEquipmentIcons(); });
        // �ӳ����˫��ж�µļ�����
        /*scheduleOnce([this, armorIcon](float) {
            if (armorIcon) {  // ȷ�� armorIcon ��Ȼ��Ч
                addDoubleClickListener(armorIcon, [this]() {
                    player->unequipArmor();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "armorListener");
    */
    }

    // ������Ʒͼ��
    if (accessory) {
        std::string accessoryFilePath = "Equipment/" + accessory->getName() + ".jpg";
        auto accessoryIcon = Sprite::create(accessoryFilePath);
        accessoryIcon->setPosition(Vec2(startX, startY - 220));
        accessoryIcon->setScale(1.3f);
        equipmentIconsContainer->addChild(accessoryIcon);
        addHoverListenerForIcons(accessoryIcon, accessory.get()->getName(), std::to_string(accessory.get()->getPower()), accessory.get()->getId(), [this, accessoryIcon]() { player->unequipAccessory();
        this->eraseHoverListenerForIcons(accessoryIcon); this->refreshEquipmentIcons(); });
        /* �ӳ����˫��ж�µļ�����
        scheduleOnce([this, accessoryIcon](float) {
            if (accessoryIcon) {  // ȷ�� accessoryIcon ��Ȼ��Ч
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


// �����ð�Χ�л���
void BackpackMainLayer::createBoundingBoxForIcons(cocos2d::Sprite* sprite) {
    if (!sprite) return;

    // ��ȡ BoundingBox
    cocos2d::Rect box = sprite->getBoundingBox();

    // ����һ�� DrawNode ���ư�Χ��
    auto drawNode = cocos2d::DrawNode::create();
    drawNode->drawRect(cocos2d::Vec2(box.origin.x, box.origin.y),
        cocos2d::Vec2(box.origin.x + box.size.width, box.origin.y + box.size.height),
        cocos2d::Color4F::RED);

    // ��ӵ����ڵ�
    this->addChild(drawNode);
}

void BackpackMainLayer::onLeftArrowClicked() {
    // ���ͷ����߼����л��������
    int element = static_cast<int>(player->getElement());
    element = (element + 6) % 7;
    player->chanegElement(static_cast<Element>(element));
    createPlayerDataDisplay();
    // ����Ҫ����������仯
    CCLOG("Left arrow action triggered!");
}

void BackpackMainLayer::onRightArrowClicked() {
    // �Ҽ�ͷ����߼����л��������
    int element = static_cast<int>(player->getElement());
    element = (element + 8) % 7;
    player->chanegElement(static_cast<Element>(element));
    createPlayerDataDisplay();
    // ����Ҫ����������仯
    CCLOG("Right arrow action triggered!");
}

void BackpackMainLayer::createBackpackUI() {
    // ����һ����ǩ��ʾ��������ı���
    auto label = Label::createWithSystemFont("Backpack", "Arial", 24);
    label->setPosition(Vec2(1200, 1050));
    this->addChild(label);

    // ��Ʒ���ӵ���ʼλ��
    float startX = 700.0f;
    float startY = 960.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 9;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;

    // ������ƽ�̸���
    for (int i = 0; i < 63; ++i) { 
        // ���㵱ǰ���ӵ�λ��
        float x = startX + (i % itemsPerRow) * (itemWidth + 25); // ÿ������֮���� 5 �ļ��
        float y = startY - (i / itemsPerRow) * (itemHeight + 25); // ÿ�еĸ��Ӽ�� 5

        // �������ӵ� Sprite ������λ��
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        itemSprite->setScale(1.3f);
        this->addChild(itemSprite);
    }
}

// ��ӵ������ͣ��������
void BackpackMainLayer::addHoverListenerForIcons(Sprite* icon, const std::string& name, const std::string& effectValue, int id, std::function<void()> cb) {

    // ��ʾ��Ʒ�����ƺ�Ч��
    int type = id / 100000;
    int subType = id % 1000 / 100;
    std::string effect;
    // ��ȡͼ��ı߽�
    Rect boundingBox = icon->getBoundingBox();
    // ����boundingBox��ƫ��
    if (type != 9) {
        boundingBox.origin = boundingBox.origin - Vec2(boundingBox.size.width, 0);
    }
    else {
        boundingBox.origin = boundingBox.origin - Vec2(boundingBox.size.width/2, 0);
    }
    switch (type) {
    case 1: // װ����
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
    case 2: // ҩ����
        effect = effectValue + " Stamina  regen";
        break;
    case 3: // ʳ����
        effect = effectValue + " Health  regen";
        break;
    case 9: // ����
        effect = "StaminaCost: " + effectValue;
        break;
    default:
        CCLOG("Error: Unsupported type %d for ID %d", type, id);
        break;
    }
    // ������Ʒ��������Ч����Ϣ
    HoverInfo hoverInfo;
    hoverInfo.name = name;
    hoverInfo.effectValue = effect;
    hoverInfo.callback = cb;
    // ���� Rect ��λ�û��ֵ���Ӧ����
    if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() > 490) {
        // ��������
        topLeftGrid[boundingBox] = hoverInfo;
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() > 490) {
        // ��������
        topRightGrid[boundingBox] = hoverInfo;
    }
    else if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() <= 490) {
        // ��������
        bottomLeftGrid[boundingBox] = hoverInfo;
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() <= 490) {
        // ��������
        bottomRightGrid[boundingBox] = hoverInfo;
    }
}

// �Ƴ������ͣ��������
void BackpackMainLayer::eraseHoverListenerForIcons(Sprite* icon) {

    // ��ȡͼ��ı߽�
    Rect boundingBox = icon->getBoundingBox();
    // ����boundingBox��ƫ��
    boundingBox.origin = boundingBox.origin - Vec2(boundingBox.size.width, 0);
    if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() > 490) {
        // ��������
        topLeftGrid.erase(boundingBox);
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() > 490) {
        // ��������
        topRightGrid.erase(boundingBox);
    }
    else if (boundingBox.getMidX() <= 1160 && boundingBox.getMidY() <= 490) {
        // ��������
        bottomLeftGrid.erase(boundingBox);
    }
    else if (boundingBox.getMidX() > 1160 && boundingBox.getMidY() <= 490) {
        // ��������
        bottomRightGrid.erase(boundingBox);
    }
}

// �������㲿�ֵļ�����Ϣ��ʾ
void BackpackMainLayer::showHoverInfo(const std::string& name, const std::string& effectValue, const Vec2& position) {
    // ����һ������ LayerColor
    if (!_hoverLabelBackground) {
        _hoverLabelBackground = LayerColor::create(Color4B(0, 0, 0, 180));  // ��ɫ��͸������
        _hoverLabelBackground->setContentSize(Size(200, 100));  // ���ñ����Ĵ�С�����Ը�����Ҫ����
        this->addChild(_hoverLabelBackground);
    }

    // ����һ�� Label ��ʾ��ͣ��Ϣ
    if (!_hoverLabel) {
        _hoverLabel = Label::createWithSystemFont("", "Arial", 24);
        _hoverLabel->setColor(Color3B::WHITE);
        _hoverLabelBackground->addChild(_hoverLabel);  // �� Label ��ӵ�������
    }
    _hoverLabelBackground->setVisible(true);
    // ���� Label ������
    std::string hoverText = name + "\n" + effectValue;
    _hoverLabel->setString(hoverText);

    // ���±�����С
    _hoverLabelBackground->setContentSize(Size(_hoverLabel->getContentSize().width + 20, _hoverLabel->getContentSize().height + 20));

    // ���±������ı���λ��
    _hoverLabelBackground->setPosition(position + Vec2(50, 0)); // ����λ��
    _hoverLabel->setPosition(Vec2(_hoverLabelBackground->getContentSize().width / 2, _hoverLabelBackground->getContentSize().height / 2)); // ���־���
}

// ������ͣ��Ϣ
void BackpackMainLayer::hideHoverInfo() {
    if (mouseArea==0) {
        // ��������
        for (const auto entry : topLeftGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    else if (mouseArea == 1) {        
        // ��������
        for (const auto& entry : topRightGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    else if (mouseArea = 2) {
        // ��������
        for (const auto& entry : bottomLeftGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    else if (mouseArea == 3) {       
        // ��������
        for (const auto& entry : topRightGrid) {
            if (entry.second.isHovering == true) {
                return;
            }
        }
    }
    if (_hoverLabel) {
        _hoverLabel->setString("");  // ����ı�
    }
    if (_hoverLabelBackground) {
        _hoverLabelBackground->setVisible(false);  // ���ر���
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
    static const float DOUBLE_CLICK_INTERVAL = 0.2f; // ˫����ʱ�������룩
    static float lastClickTime = 0.0f; // ��һ�ε��ʱ��

    auto listener = EventListenerMouse::create();

    listener->onMouseMove = [this](EventMouse* event) {
        // ��ȡ���λ��
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        // ���� Rect ��λ�û��ֵ���Ӧ����
        if (cursorPosition.x <= 1160 && cursorPosition.y > 490) {
            mouseArea = 0;
            // ��������
            for (auto entry : topLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    entry.second.isHovering = true;  // ���µ�ǰ��ͣ��Ϣ
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
            // ��������
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    entry.second.isHovering = true;  // ���µ�ǰ��ͣ��Ϣ
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }
        else if (cursorPosition.x <= 1160 && cursorPosition.y <= 490) {
            mouseArea = 2;
            // ��������
            for (auto& entry : bottomLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    entry.second.isHovering = true;  // ���µ�ǰ��ͣ��Ϣ
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y <= 490) {
            mouseArea = 3;
            // ��������
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    showHoverInfo(entry.second.name, entry.second.effectValue, cursorPosition);
                    entry.second.isHovering = true;  // ���µ�ǰ��ͣ��Ϣ
                    return;
                }
                else {
                    entry.second.isHovering = false;
                }
            }
        }

        };

    listener->onMouseDown = [this](EventMouse* event) {
        // ��ȡ���λ��
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        // ���� Rect ��λ�û��ֵ���Ӧ����
        if (cursorPosition.x <= 1160 && cursorPosition.y > 490) {
            // ��������
            for (auto entry : topLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // ˫�������ص�
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // ��¼ʱ��
                    }
                    return;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y > 490) {
            // ��������
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // ˫�������ص�
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // ��¼ʱ��
                    }
                    return;
                }
            }
        }
        else if (cursorPosition.x <= 1160 && cursorPosition.y <= 490) {
            // ��������
            for (auto& entry : bottomLeftGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // ˫�������ص�
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // ��¼ʱ��
                    }
                    return;
                }
            }
        }
        else if (cursorPosition.x > 1160 && cursorPosition.y <= 490) {
            // ��������
            for (auto& entry : topRightGrid) {
                if (entry.first.containsPoint(cursorPosition)) {
                    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
                    if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                        if (entry.second.callback) {
                            entry.second.callback(); // ˫�������ص�
                        }
                        lastClickTime = 0.0f;
                    }
                    else {
                        lastClickTime = currentTime; // ��¼ʱ��
                    }
                    return;
                }
            }
        }

        };
    // ע�������
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}