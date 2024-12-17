#include "SkillLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

SkillLayer::SkillLayer() {}

SkillLayer::~SkillLayer() {}

bool SkillLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    // ��ȡ��ǰ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // ���� SkillLayer �Ĵ�СΪ��Ļ����벿��
    this->setContentSize(Size(visibleSize.width / 2, visibleSize.height));

    // �� SkillLayer ��ê����Ϊ���½ǣ�������λ������Ϊ��Ļ�����
    this->setAnchorPoint(Vec2::ZERO);
    this->setPosition(origin);

    // �������ܽ����UI���
    createSkillUI();

    return true;
}

void SkillLayer::createSkillUI() {
    // ����һ����ǩ��ʾ���ܽ���ı���
    auto label = Label::createWithSystemFont("Skills", "Arial", 24);
    label->setPosition(Vec2(300, 100));
    this->addChild(label);

    // ��Ʒ���ӵ���ʼλ��
    float startX = 90.0f;
    float startY = 185.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 5;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;

    // ������ƽ�̸���
    for (int i = 0; i < 5; ++i) { // ������Ҫ���� 20 ������
        // ���㵱ǰ���ӵ�λ��
        float x = startX + (i % itemsPerRow) * (itemWidth + 5); // ÿ������֮���� 5 �ļ��
        float y = startY - (i / itemsPerRow) * (itemHeight + 5); // ÿ�еĸ��Ӽ�� 5

        // �������ӵ� Sprite ������λ��
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        itemSprite->setScale(1.3f);
        this->addChild(itemSprite);        
    }
    // ������Ը�����������������UIԪ�أ��缼�ܰ�ť������ͼ���
    
}

void SkillLayer::loadSkills() {
    unlockedSkills = player->getunlockedSkills();
    skillBar = player->getskillBar();
}

void SkillLayer::loadSkillsIcon() {
    float startX = 500;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    auto mainLayer = dynamic_cast<BackpackMainLayer*>(this->getParent());

    for (int i = 0; i < skillBar.size(); i++) {
        Skill* skill = skillBar.at(i).get();
        auto skillIcon = Sprite::create("skill/" + skill->getSkillName() + ".png");
        skillIcon->setPosition(Vec2(startX, startY - i * 120));
        skillIcon->setScale(1.5);
        this->addChild(skillIcon);
        mainLayer->addHoverListenerForIcons(skillIcon, skill->getSkillName(), std::to_string(skill->getStaminaCost()) + "\nCD: " + std::to_string(skill->getCD()), skill->getId());
    }
    
}

void SkillLayer::setPlayer(Player* player) {
    this->player = player;
    if (player) {
        // ȷ����������Ѿ�����

    // ���ؼ��ܺ����ɼ���ͼƬ
        loadSkills();
        loadSkillsIcon();
    }
}