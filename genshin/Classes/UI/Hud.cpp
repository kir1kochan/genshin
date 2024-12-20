#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"

// 构造函数，初始化各成员变量
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f) {
    // 创建 DrawNode 用于显示血条和体力条
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // 将血条和体力条节点添加为子节点
    addChild(healthBarNode);
    addChild(staminaBarNode);

    // 初始化技能栏节点
    for (int i = 0; i < 4; ++i) {
        skillBarNode.push_back(cocos2d::DrawNode::create());
        this->addChild(skillBarNode[i]);

        // 初始化技能冷却进度条（蒙版）
        cocos2d::Sprite* cooldownSprite = cocos2d::Sprite::create("imageSkill/Covery.jpg");
        skillCooldownBars.push_back(cocos2d::ProgressTimer::create(cooldownSprite));
        skillCooldownBars[i]->setType(cocos2d::ProgressTimer::Type::RADIAL);
        skillCooldownBars[i]->setMidpoint(cocos2d::Vec2(0.5f, 0.5f));  // 设置为环形进度条
        skillCooldownBars[i]->setBarChangeRate(cocos2d::Vec2(1, 1));
        this->addChild(skillCooldownBars[i]);

        // 初始隐藏冷却进度条
        skillCooldownBars[i]->setVisible(false);
    }


    // 设置初始 HUD 的位置
    setPosition(cocos2d::Vec2(0, 100));  // HUD 相对于玩家的顶部
    // 设置技能栏边框
    for (int i = 0; i < skillBarNode.size(); ++i) {
        skillBarNode[i]->drawRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(50, 50), cocos2d::Color4F::BLACK);
    }


    // 初始化进度条的相对位置
    updateBarPositions();
}

// 析构函数
Hud::~Hud() {
}

// 静态创建方法
Hud* Hud::create(Player* player) {
    Hud* hud = new (std::nothrow) Hud(player);
    if (hud && hud->init()) {
        hud->autorelease();
        return hud;
    }
    CC_SAFE_DELETE(hud);
    return nullptr;
}

// 每帧更新 HUD 的状态
void Hud::update(float deltaTime) {
    // 根据玩家当前状态更新血条和体力条
    updateHealthBar(player->getHealth() / player->getMaxHealth());
    updateStaminaBar(player->getStamina() / player->getMaxStamina());

    // 更新进度条的位置
    updateBarPositions();
}

// 更新血条显示
void Hud::updateHealthBar(float healthPercent) {
    drawProgressBar(healthBarNode, healthPercent, cocos2d::Color4F::RED);
}

// 更新体力条显示
void Hud::updateStaminaBar(float staminaPercent) {
    drawProgressBar(staminaBarNode, staminaPercent, cocos2d::Color4F::GREEN);
}

void Hud::equipSkill(int skillSlot, std::string skillName)
{
    // 为技能槽装备技能
    std::string iconPath = "imageSkill/" + skillName + ".png";
    cocos2d::Sprite* skillIcon = cocos2d::Sprite::create(iconPath);

    if (skillIcon) {
        // 获取技能栏的位置
        cocos2d::Vec2 skillBarPosition = skillBarNode[skillSlot]->getPosition();

        // 设置技能图标的大小（使图标适应50x50的边框）
        skillIcon->setScale(50.0f / skillIcon->getContentSize().width, 50.0f / skillIcon->getContentSize().height);

        // 设置技能图标的锚点为右上角
        skillIcon->setAnchorPoint(cocos2d::Vec2(0, 0));  // 锚点设为中心对齐

        // 将技能图标设置为居中在技能栏边框内
        skillIcon->setPosition(skillBarPosition);

        // 添加技能图标到 HUD 中
        this->addChild(skillIcon);

        // 确保技能图标在进度条上方
        skillIcon->setLocalZOrder(0);  // 将技能图标置于上方
    }

    // 显示技能冷却条蒙版
    skillCooldownBars[skillSlot]->setVisible(true);

    // 设置冷却进度条的尺寸与技能图标一致（50x50）
    skillCooldownBars[skillSlot]->setScale(6.45f / skillIcon->getContentSize().width, 6.45f / skillIcon->getContentSize().height);

    skillCooldownBars[skillSlot]->setOpacity(150);
    skillCooldownBars[skillSlot]->setAnchorPoint(cocos2d::Vec2(0, 0));  // 锚点设为中心对齐
    // 设置冷却进度条位置与技能图标一致
    skillCooldownBars[skillSlot]->setPosition(skillBarNode[skillSlot]->getPosition());

    // 确保冷却进度条在技能图标下方（设置较低的 z-index）
    skillCooldownBars[skillSlot]->setLocalZOrder(1);  // 进度条在技能图标下方
}


void Hud::unequipSkill(int skillSlot) {
    // 卸载技能图标
    skillBarNode[skillSlot]->removeAllChildren(); // 清除技能图标
    skillCooldownBars[skillSlot]->setVisible(false); // 隐藏技能冷却进度条
}

void Hud::useSkill(int skillSlot, float cdTime) {
    skillCooldownBars[skillSlot]->setLocalZOrder(2);  // 进度条在技能图标下方
    // 使用技能时启动冷却进度条
    skillCooldownBars[skillSlot]->setPercentage(100); // 初始化为100%

    // 动态更新冷却进度条
    cocos2d::Action* cooldownAction = cocos2d::ProgressTo::create(2*cdTime, 0);
    skillCooldownBars[skillSlot]->runAction(cooldownAction);
    skillCooldownBars[skillSlot]->setLocalZOrder(0);  // 进度条在技能图标下方
}



// 绘制进度条
void Hud::drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color) {
    barNode->clear(); // 清空旧的内容

    // 确保百分比在 0 到 1 之间
    percent = std::max(0.0f, std::min(1.0f, percent));
    float filledWidth = barWidth * percent;

    // 绘制矩形进度条
    barNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(filledWidth, barHeight), color);

    // 绘制边框
    cocos2d::Vec2 origin(0, 0); // 起点
    cocos2d::Vec2 topRight(barWidth, barHeight); // 右上角点
    barNode->drawRect(origin, topRight, cocos2d::Color4F::BLACK); // 黑色边框
}

// 设置进度条的相对位置
void Hud::updateBarPositions() {
    float offsetX = -barWidth - 300; // 距离玩家左侧的偏移量
    float offsetY = -375;           // 距离玩家底部的偏移量

    // 设置每个条的位置
    healthBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY + 30)); // 血条（稍上）
    staminaBarNode->setPosition(cocos2d::Vec2(offsetX, offsetY));     // 体力条（稍下）

    // 设置技能栏位置
    for (int i = 0; i < skillBarNode.size(); ++i) {
        skillBarNode[i]->setPosition(cocos2d::Vec2(offsetX+300+60*i, offsetY));
        skillCooldownBars[i]->setPosition(cocos2d::Vec2(offsetX + 300 + 60 * i, offsetY));
    }
}
