#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"

// ���캯������ʼ������Ա����
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f) {
    // ���� DrawNode ������ʾѪ����������
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // ��Ѫ�����������ڵ����Ϊ�ӽڵ�
    addChild(healthBarNode);
    addChild(staminaBarNode);

    // ��ʼ���������ڵ�
    for (int i = 0; i < 4; ++i) {
        skillBarNode.push_back(cocos2d::DrawNode::create());
        this->addChild(skillBarNode[i]);

        // ��ʼ��������ȴ���������ɰ棩
        cocos2d::Sprite* cooldownSprite = cocos2d::Sprite::create("imageSkill/Covery.jpg");
        skillCooldownBars.push_back(cocos2d::ProgressTimer::create(cooldownSprite));
        skillCooldownBars[i]->setType(cocos2d::ProgressTimer::Type::RADIAL);
        skillCooldownBars[i]->setMidpoint(cocos2d::Vec2(0.5f, 0.5f));  // ����Ϊ���ν�����
        skillCooldownBars[i]->setBarChangeRate(cocos2d::Vec2(1, 1));
        this->addChild(skillCooldownBars[i]);

        // ��ʼ������ȴ������
        skillCooldownBars[i]->setVisible(false);
    }


    // ���ó�ʼ HUD ��λ��
    setPosition(cocos2d::Vec2(0, 100));  // HUD �������ҵĶ���
    // ���ü������߿�
    for (int i = 0; i < skillBarNode.size(); ++i) {
        skillBarNode[i]->drawRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(50, 50), cocos2d::Color4F::BLACK);
    }


    // ��ʼ�������������λ��
    updateBarPositions();
}

// ��������
Hud::~Hud() {
}

// ��̬��������
Hud* Hud::create(Player* player) {
    Hud* hud = new (std::nothrow) Hud(player);
    if (hud && hud->init()) {
        hud->autorelease();
        return hud;
    }
    CC_SAFE_DELETE(hud);
    return nullptr;
}

// ÿ֡���� HUD ��״̬
void Hud::update(float deltaTime) {
    // ������ҵ�ǰ״̬����Ѫ����������
    updateHealthBar(player->getHealth() / player->getMaxHealth());
    updateStaminaBar(player->getStamina() / player->getMaxStamina());

    // ���½�������λ��
    updateBarPositions();
}

// ����Ѫ����ʾ
void Hud::updateHealthBar(float healthPercent) {
    drawProgressBar(healthBarNode, healthPercent, cocos2d::Color4F::RED);
}

// ������������ʾ
void Hud::updateStaminaBar(float staminaPercent) {
    drawProgressBar(staminaBarNode, staminaPercent, cocos2d::Color4F::GREEN);
}

void Hud::equipSkill(int skillSlot, std::string skillName)
{
    // Ϊ���ܲ�װ������
    std::string iconPath = "imageSkill/" + skillName + ".png";
    cocos2d::Sprite* skillIcon = cocos2d::Sprite::create(iconPath);

    if (skillIcon) {
        // ��ȡ��������λ��
        cocos2d::Vec2 skillBarPosition = skillBarNode[skillSlot]->getPosition();

        // ���ü���ͼ��Ĵ�С��ʹͼ����Ӧ50x50�ı߿�
        skillIcon->setScale(50.0f / skillIcon->getContentSize().width, 50.0f / skillIcon->getContentSize().height);

        // ���ü���ͼ���ê��Ϊ���Ͻ�
        skillIcon->setAnchorPoint(cocos2d::Vec2(0, 0));  // ê����Ϊ���Ķ���

        // ������ͼ������Ϊ�����ڼ������߿���
        skillIcon->setPosition(skillBarPosition);

        // ��Ӽ���ͼ�굽 HUD ��
        this->addChild(skillIcon);

        // ȷ������ͼ���ڽ������Ϸ�
        skillIcon->setLocalZOrder(0);  // ������ͼ�������Ϸ�
    }

    // ��ʾ������ȴ���ɰ�
    skillCooldownBars[skillSlot]->setVisible(true);

    // ������ȴ�������ĳߴ��뼼��ͼ��һ�£�50x50��
    skillCooldownBars[skillSlot]->setScale(6.45f / skillIcon->getContentSize().width, 6.45f / skillIcon->getContentSize().height);

    skillCooldownBars[skillSlot]->setOpacity(150);
    skillCooldownBars[skillSlot]->setAnchorPoint(cocos2d::Vec2(0, 0));  // ê����Ϊ���Ķ���
    // ������ȴ������λ���뼼��ͼ��һ��
    skillCooldownBars[skillSlot]->setPosition(skillBarNode[skillSlot]->getPosition());

    // ȷ����ȴ�������ڼ���ͼ���·������ýϵ͵� z-index��
    skillCooldownBars[skillSlot]->setLocalZOrder(1);  // �������ڼ���ͼ���·�
}


void Hud::unequipSkill(int skillSlot) {
    // ж�ؼ���ͼ��
    skillBarNode[skillSlot]->removeAllChildren(); // �������ͼ��
    skillCooldownBars[skillSlot]->setVisible(false); // ���ؼ�����ȴ������
}

void Hud::useSkill(int skillSlot, float cdTime) {
    skillCooldownBars[skillSlot]->setLocalZOrder(2);  // �������ڼ���ͼ���·�
    // ʹ�ü���ʱ������ȴ������
    skillCooldownBars[skillSlot]->setPercentage(100); // ��ʼ��Ϊ100%

    // ��̬������ȴ������
    cocos2d::Action* cooldownAction = cocos2d::ProgressTo::create(2*cdTime, 0);
    skillCooldownBars[skillSlot]->runAction(cooldownAction);
    skillCooldownBars[skillSlot]->setLocalZOrder(0);  // �������ڼ���ͼ���·�
}



// ���ƽ�����
void Hud::drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color) {
    barNode->clear(); // ��վɵ�����

    // ȷ���ٷֱ��� 0 �� 1 ֮��
    percent = std::max(0.0f, std::min(1.0f, percent));
    float filledWidth = barWidth * percent;

    // ���ƾ��ν�����
    barNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(filledWidth, barHeight), color);

    // ���Ʊ߿�
    cocos2d::Vec2 origin(0, 0); // ���
    cocos2d::Vec2 topRight(barWidth, barHeight); // ���Ͻǵ�
    barNode->drawRect(origin, topRight, cocos2d::Color4F::BLACK); // ��ɫ�߿�
}

// ���ý����������λ��
void Hud::updateBarPositions() {
    float offsetX = -barWidth - 300; // �����������ƫ����
    float offsetY = -375;           // ������ҵײ���ƫ����

    // ����ÿ������λ��
    healthBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY + 30)); // Ѫ�������ϣ�
    staminaBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY));     // �����������£�

    // ���ü�����λ��
    for (int i = 0; i < skillBarNode.size(); ++i) {
        skillBarNode[i]->setPosition(cocos2d::Vec2(offsetX+300+60*i, offsetY));
        skillCooldownBars[i]->setPosition(cocos2d::Vec2(offsetX + 300 + 60 * i, offsetY));
    }
}
