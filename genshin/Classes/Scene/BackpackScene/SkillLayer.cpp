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
    // 获取当前屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 设置 SkillLayer 的大小为屏幕的左半部分
    this->setContentSize(Size(visibleSize.width / 3, visibleSize.height));

    // 将 SkillLayer 的锚点设为左下角，并将其位置设置为屏幕的左侧
    this->setAnchorPoint(Vec2::ZERO);
    this->setPosition(origin);

    // 创建技能界面的UI组件
    createSkillUI();

    return true;
}

void SkillLayer::createSkillUI() {
    // 创建一个标签显示技能界面的标题
    auto label = Label::createWithSystemFont("Skills", "Arial", 24);
    label->setPosition(Vec2(300, 100));
    this->addChild(label);

    // 物品格子的起始位置
    float startX = 90.0f;
    float startY = 185.0f;

    // 每行显示多少个格子
    int itemsPerRow = 5;
    float itemWidth = 110.0f;
    float itemHeight = 110.0f;

    // 创建并平铺格子
    for (int i = 0; i < 5; ++i) { // 假设你要创建 20 个格子
        // 计算当前格子的位置
        float x = startX + (i % itemsPerRow) * (itemWidth + 5); // 每个格子之间有 5 的间隔
        float y = startY - (i / itemsPerRow) * (itemHeight + 5); // 每行的格子间隔 5

        // 创建格子的 Sprite 并设置位置
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        itemSprite->setScale(1.3f);
        this->addChild(itemSprite);        
    }
    // 这里可以根据需求继续添加其他UI元素，如技能按钮、技能图标等
    
}

void SkillLayer::loadSkills() {
    unlockedSkills = player->getunlockedSkills();
    skillBar = player->getskillBar();
    availableSkills.clear();
    // 遍历 unlockedSkills
    for (const auto& unlockedSkill : unlockedSkills) {
        // 检查 unlockedSkill 是否在 skillBar 中
        auto it = std::find_if(skillBar.begin(), skillBar.end(),
            [&unlockedSkill](const std::shared_ptr<Skill>& skill) {
                return skill && skill->getSkillName() == unlockedSkill->getSkillName();
            });

        // 如果 skillBar 中没有该技能，则添加到 newSkills
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
            Size originalSize = skillIcon->getContentSize(); // 获取图标的原始大小

            // 计算缩放比例
            float scaleX = 72.0f / originalSize.width;
            float scaleY = 72.0f / originalSize.height;

            // 设置缩放
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
            Size originalSize = skillIcon->getContentSize(); // 获取图标的原始大小

            // 计算缩放比例
            float scaleX = 72.0f / originalSize.width;
            float scaleY = 72.0f / originalSize.height;

            // 设置缩放
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
        // 确保玩家数据已经设置

    // 加载技能和生成技能图片
        loadSkills();
        loadSkillsIcon();
    }
}

void SkillLayer::update() {
    if (player) {
        // 确保玩家数据已经设置

    // 加载技能和生成技能图片
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
        availableSkills.at(skillToEquip).get()->resetCooldown(); // 替换技能后需要重新冷却
        skillToEquip = -1;
        skillToUnload = -1;
        this->update();
    }
    else if (skillToUnload != -1) {
        this->removeChildByName("highlight");
        // 创建一个黄色半透明的 Layer
        auto yellowLayer = LayerColor::create(Color4B(255, 255, 0, 128), 72, 72); // 设置颜色为黄色（255, 255, 0），透明度为128，大小为72x72
        yellowLayer->setPosition(Vec2(startX, startY - skillToUnload * 110) - Vec2(36, 36)); // 设置位置
        yellowLayer->setName("highlight");
        this->addChild(yellowLayer); // 将 Layer 添加到当前 Layer
    }
    else if (skillToEquip != -1) {
        startX = 90.0f;
        startY = 185.0f;
        this->removeChildByName("highlight");
        // 创建一个黄色半透明的 Layer
        auto yellowLayer = LayerColor::create(Color4B(255, 255, 0, 128), 72, 72); // 设置颜色为黄色（255, 255, 0），透明度为128，大小为72x72
        yellowLayer->setPosition(Vec2(startX + skillToEquip * 115, startY) - Vec2(36, 36)); // 设置位置
        yellowLayer->setName("highlight");
        this->addChild(yellowLayer); // 将 Layer 添加到当前 Layer
    }
}