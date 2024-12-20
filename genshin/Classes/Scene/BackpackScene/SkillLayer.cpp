#include "SkillLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"
#include "Common/Entities/Player/Skill/AttackSkill/AttackSkill.h"
#include "Common/Entities/Player/Skill/ShieldSkill/ShieldSkill.h"
#include "Common/Entities/Player/Skill/HealSkill/HealSkill.h"
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
    this->setContentSize(Size(visibleSize.width / 3, visibleSize.height));

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
    availableSkills.clear();
    // ���� unlockedSkills
    for (const auto& unlockedSkill : unlockedSkills) {
        // ��� unlockedSkill �Ƿ��� skillBar ��
        auto it = std::find_if(skillBar.begin(), skillBar.end(),
            [&unlockedSkill](const std::shared_ptr<Skill>& skill) {
                return skill && skill->getSkillName() == unlockedSkill->getSkillName();
            });

        // ��� skillBar ��û�иü��ܣ�����ӵ� newSkills
        if (it == skillBar.end()) {
            availableSkills.push_back(unlockedSkill);
        }
    }
}

void SkillLayer::loadSkillsIcon() {
    float startX = 500;
    float startY = Director::getInstance()->getVisibleSize().height - 100;

    auto mainLayer = dynamic_cast<BackpackMainLayer*>(this->getParent());

    for (int i = 0; i < skillBar.size(); i++) {
        if (skillBar[i]) {
            Skill* skill = skillBar.at(i).get();
            auto skillIcon = Sprite::create("Skill/" + skill->getSkillName() + ".png");
            skillIcon->setPosition(Vec2(startX, startY - i * 110));
            Size originalSize = skillIcon->getContentSize(); // ��ȡͼ���ԭʼ��С

            // �������ű���
            float scaleX = 72.0f / originalSize.width;
            float scaleY = 72.0f / originalSize.height;

            // ��������
            skillIcon->setScale(scaleX, scaleY);
            skillIcon->setTag(100 + i);
            this->addChild(skillIcon); std::string power = "";
            int type = skill->getId() % 1000 / 100;
            if (type == 1) {
                auto attackSkill = dynamic_cast<AttackSkill*>(skill);
                power += "Power: " + std::to_string((int)attackSkill->getAttackPower());
            }
            else if (type == 2) {
                auto shieldSkill = dynamic_cast<ShieldSkill*>(skill);
                power += "Shield value:" + std::to_string((int)shieldSkill->getShieldValue()) ;
            }
            else {
                auto healSkill = dynamic_cast<HealSkill*>(skill);
                power += "Heal amount: " + std::to_string((int)healSkill->getHealAmount());
            }
            mainLayer->addHoverListenerForIcons(skillIcon, skill->getSkillName() + "\n" + power, std::to_string((int)skill->getStaminaCost()) + "\nCD: " + std::to_string(skill->getCD()).substr(0, 3),
                skill->getId(), [this, i]() {this->setSkillToUnload(i);  });
        }
    }

    startX = 90.0f;
    startY = 185.0f;
    for (int i = 0; i < availableSkills.size(); i++) {
        if (availableSkills[i]) {
            Skill* skill = availableSkills.at(i).get();
            auto skillIcon = Sprite::create("imageSkill/" + skill->getSkillName() + ".png");
            Size originalSize = skillIcon->getContentSize(); // ��ȡͼ���ԭʼ��С

            // �������ű���
            float scaleX = 72.0f / originalSize.width;
            float scaleY = 72.0f / originalSize.height;

            // ��������
            skillIcon->setScale(scaleX, scaleY);
            skillIcon->setPosition(Vec2(startX + i * 115, startY));
            skillIcon->setTag(200 + i);
            this->addChild(skillIcon);
            std::string power = "";
            int type = skill->getId() % 1000 / 100;
            if (type == 1) {
                auto attackSkill = dynamic_cast<AttackSkill*>(skill);
                power += "Power: " + std::to_string((int)attackSkill->getAttackPower());
            }
            else if (type == 2) {
                auto shieldSkill = dynamic_cast<ShieldSkill*>(skill);
                power += "Shield value: " + std::to_string((int)shieldSkill->getShieldValue()) ;
            }
            else {
                auto healSkill = dynamic_cast<HealSkill*>(skill);
                power += "Heal amount: " + std::to_string((int)healSkill->getHealAmount());
            }
            mainLayer->addHoverListenerForIcons(skillIcon, skill->getSkillName() + "\n" + power, std::to_string((int)skill->getStaminaCost()) + "\nCD: " + std::to_string(skill->getCD()).substr(0, 3),
                skill->getId(), [this, i]() {this->setSkillToEquip(i); });
        }
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

void SkillLayer::update() {
    if (player) {
        // ȷ����������Ѿ�����

    // ���ؼ��ܺ����ɼ���ͼƬ
        loadSkills();
        loadSkillsIcon();
    }
}

void SkillLayer::setSkillToUnload(int i) {
    if (skillToUnload == i) {
        skillToUnload = -1;
    }
    skillToUnload = i;
}

void SkillLayer::setSkillToEquip(int i) {
    if (skillToEquip == i) {
        skillToEquip = -1;
    }
    skillToEquip = i;
}

void SkillLayer::updateSkills() {
    float startX = 500;
    float startY = Director::getInstance()->getVisibleSize().height - 100;
    if (skillToEquip != -1 && skillToUnload != -1) {
        player->unequipSkill(skillToUnload);
        player->equipSkill(skillToUnload, availableSkills.at(skillToEquip));
        availableSkills.at(skillToEquip).get()->resetCooldown(); // �滻���ܺ���Ҫ������ȴ
        skillToEquip = -1;
        skillToUnload = -1;
        this->update();
    }
    else if (skillToUnload != -1) {
        this->removeChildByName("highlight");
        // ����һ����ɫ��͸���� Layer
        auto yellowLayer = LayerColor::create(Color4B(255, 255, 0, 128), 72, 72); // ������ɫΪ��ɫ��255, 255, 0����͸����Ϊ128����СΪ72x72
        yellowLayer->setPosition(Vec2(startX, startY - skillToUnload * 110) - Vec2(36, 36)); // ����λ��
        yellowLayer->setName("highlight");
        this->addChild(yellowLayer); // �� Layer ��ӵ���ǰ Layer
    }
    else if (skillToEquip != -1) {
        startX = 90.0f;
        startY = 185.0f;
        this->removeChildByName("highlight");
        // ����һ����ɫ��͸���� Layer
        auto yellowLayer = LayerColor::create(Color4B(255, 255, 0, 128), 72, 72); // ������ɫΪ��ɫ��255, 255, 0����͸����Ϊ128����СΪ72x72
        yellowLayer->setPosition(Vec2(startX + skillToEquip * 115, startY) - Vec2(36, 36)); // ����λ��
        yellowLayer->setName("highlight");
        this->addChild(yellowLayer); // �� Layer ��ӵ���ǰ Layer
    }
}