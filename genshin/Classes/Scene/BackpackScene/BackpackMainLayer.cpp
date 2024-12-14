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
    auto background = LayerColor::create(Color4B(139, 69, 19, 255)); // ��ɫ����,�����޸ĳɱ�ı���
    this->addChild(background);

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
        equipmentIconsContainer->addChild(weaponIcon);
        // ��ʾ��Χ��
        createBoundingBoxForIcons(weaponIcon);

        // �ӳ����˫��ж�µļ�����
        scheduleOnce([this, weaponIcon](float) {
            if (weaponIcon) {  // ȷ�� weaponIcon ��Ȼ��Ч
                addDoubleClickListener(weaponIcon, [this]() {
                    player->unequipWeapon();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "weaponListener");
    }

    // ���ɿ���ͼ��
    if (armor) {
        std::string armorFilePath = "Equipment/" + armor->getName() + ".jpg";
        auto armorIcon = Sprite::create(armorFilePath);
        armorIcon->setPosition(Vec2(startX, startY - 90));
        equipmentIconsContainer->addChild(armorIcon);

        // �ӳ����˫��ж�µļ�����
        scheduleOnce([this, armorIcon](float) {
            if (armorIcon) {  // ȷ�� armorIcon ��Ȼ��Ч
                addDoubleClickListener(armorIcon, [this]() {
                    player->unequipArmor();
                    this->refreshEquipmentIcons();
                    });
            }
            }, 1.0f, "armorListener");
    }

    // ������Ʒͼ��
    if (accessory) {
        std::string accessoryFilePath = "Equipment/" + accessory->getName() + ".jpg";
        auto accessoryIcon = Sprite::create(accessoryFilePath);
        accessoryIcon->setPosition(Vec2(startX, startY - 180));
        equipmentIconsContainer->addChild(accessoryIcon);

        // �ӳ����˫��ж�µļ�����
        scheduleOnce([this, accessoryIcon](float) {
            if (accessoryIcon) {  // ȷ�� accessoryIcon ��Ȼ��Ч
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
    static const float DOUBLE_CLICK_INTERVAL = 0.3f; // ˫����ʱ�������룩
    static float lastClickTime = 0.0f; // ��һ�ε��ʱ��

    auto listener = EventListenerMouse::create();

    listener->onMouseDown = [target, callback](EventMouse* event) {
        // ��ȡ����λ�ã��������꣩
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        Vec2 locationInNode = cursorPosition;
        Rect rect = target->getBoundingBox();

        if ((locationInNode.x <= rect.origin.x && locationInNode.x >= rect.origin.x - rect.size.width) &&
            (locationInNode.y >= rect.origin.y && locationInNode.y <= rect.origin.y + rect.size.height)) {
            float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;

            if (currentTime - lastClickTime < DOUBLE_CLICK_INTERVAL) {
                callback(); // ˫�������ص�
                lastClickTime = 0.0f;
            }
            else {
                lastClickTime = currentTime; // ��¼ʱ��
            }
        }
        };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
}



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