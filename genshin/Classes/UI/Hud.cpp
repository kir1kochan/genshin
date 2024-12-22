#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "TestScene.h"
#include "Scene/SceneObject/TPAnchor.h"

// 构造函数，初始化各成员变量
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f), isMiniMapExpanded(false), tpAnchor(tpAnchor) {
    // 创建 DrawNode 用于显示血条和体力条
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // 将血条和体力条节点添加为子节点
    addChild(healthBarNode, -1);
    addChild(staminaBarNode, -1);

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

    // 创建小地图节点
    miniMapNode = cocos2d::TMXTiledMap::create("/maps/small_map2.tmx");
    miniMapNode->setScale(1.0f); // 设置小地图的缩放比例

    // 创建裁剪节点
    clipper = cocos2d::ClippingNode::create();
    clipper->setContentSize(cocos2d::Size(100, 100)); // 设置裁剪区域大小
    clipper->setPosition(cocos2d::Vec2(-500, -20)); // 设置裁剪区域位置
    addChild(clipper, 1);

    // 创建一个遮罩层
    stencil = cocos2d::DrawNode::create();
    stencil->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(100, 100), cocos2d::Color4F::WHITE);
    clipper->setStencil(stencil);

    // 将小地图添加到裁剪节点中
    clipper->addChild(miniMapNode);

    // 创建一个 DrawNode 用于绘制黄色边框
    borderNode = cocos2d::DrawNode::create();
    float borderWidth = 2.0f; // 边框厚度
    cocos2d::Vec2 rect[4] = {
        cocos2d::Vec2(-borderWidth, -borderWidth),
        cocos2d::Vec2(100 + borderWidth, -borderWidth),
        cocos2d::Vec2(100 + borderWidth, 100 + borderWidth),
        cocos2d::Vec2(-borderWidth, 100 + borderWidth)
    };
    borderNode->drawPolygon(rect, 4, cocos2d::Color4F(0, 0, 0, 0), borderWidth, cocos2d::Color4F::YELLOW);
    borderNode->setPosition(clipper->getPosition()); // 设置边框位置与裁剪节点一致
    addChild(borderNode, 2); // 将边框节点添加到 HUD 中

    // 初始化小地图玩家图标
    initMiniMapPlayerIcon();

    // 设置初始 HUD 的位置
    setPosition(cocos2d::Vec2(0, 100));  // HUD 相对于玩家的顶部

    // 初始化进度条的相对位置
    updateBarPositions();

    // 初始化小地图的位置
    updateMiniMapPosition();

    // 设置技能栏边框
    for (int i = 0; i < skillBarNode.size(); ++i) {
        skillBarNode[i]->drawRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(50, 50), cocos2d::Color4F::BLACK);
    }


    // 初始化进度条的相对位置
    updateBarPositions();

    // 注册事件监听
    auto listener2 = cocos2d::EventListenerCustom::create("MAP_ENDED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Map event ended!");
        if (isMiniMapExpanded) {
            toggleMiniMap();
        }
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);  // 将监听器添加到事件分发器
}

void Hud::setTPAnchor(TPAnchor* tpAnchor) {
    this->tpAnchor = tpAnchor;
    float scale = 32.0f / 256.0f;
    auto tpAnchors = tpAnchor->gettpPointActivation();
    miniMapNode->removeChildByName("Anchor");
    for (auto& anchor : tpAnchors) {
        Vec2 pos = anchor.first;
        if (anchor.second) {
            auto sprite = cocos2d::Sprite::create("Icon/Anchor.jpg");
            sprite->setScale(0.5);
            miniMapNode->addChild(sprite);
            sprite->setName("Anchor");
            sprite->setPosition(pos.x * scale, pos.y * scale);
        }
        else {
            auto sprite = cocos2d::Sprite::create("Icon/Anchor.jpg");
            sprite->setScale(0.5);
            miniMapNode->addChild(sprite);
            sprite->setName("Anchor");
            sprite->setColor(cocos2d::Color3B(128, 128, 128));
            sprite->setPosition(pos.x * scale, pos.y * scale);
        }
    }
}

void Hud::updateMissionIconPosition(int stage) {
    if (!expandedMiniMapNode) {
        return; // 如果小地图2未加载，则直接返回
    }
    // 获取 mission 图层
    auto missionLayer = expandedMiniMapNode->getObjectGroup("mission");
    if (!missionLayer) {
        return; // 如果 mission 图层不存在，则直接返回
    }
    // 获取主线任务图标对象
    auto missionObject = missionLayer->getObject("MissionIcon");
    if (missionObject.empty()) {
        return; // 如果主线任务图标对象不存在，则直接返回
    }
    // 获取当前图标位置
    float x = missionObject["x"].asFloat();
    float y = missionObject["y"].asFloat();
    // 根据阶段值更新图标位置
    switch (stage) {
    case 0:
        x -= 200;
        break;
    case 1:
        x += 300;
        y += 300;
        break;
        // 可以根据需要添加更多阶段
    default:
        break;
    }
    // 更新主线任务图标的位置
    auto missionIcon = expandedMiniMapNode->getChildByName<cocos2d::Sprite*>("MissionIcon");
    if (missionIcon) {
        missionIcon->setPosition(cocos2d::Vec2(x, y));
    }
    else {
        // 如果图标不存在，则创建并添加到小地图2中
        missionIcon = cocos2d::Sprite::create("mission_icon.png");
        missionIcon->setName("MissionIcon");
        missionIcon->setPosition(cocos2d::Vec2(x, y));
        expandedMiniMapNode->addChild(missionIcon);
    }
}

void Hud::updateSideMissionIconPosition(const std::string& missionName, bool isVisible) {
    if (!expandedMiniMapNode) {
        return; // 如果小地图2未加载，则直接返回
    }
    // 获取 sideMission 图层
    auto sideMissionLayer = expandedMiniMapNode->getObjectGroup("sideMission");
    if (!sideMissionLayer) {
        return; // 如果 sideMission 图层不存在，则直接返回
    }
    // 获取支线任务图标对象
    auto missionObject = sideMissionLayer->getObject(missionName);
    if (missionObject.empty()) {
        return; // 如果支线任务图标对象不存在，则直接返回
    }
    // 获取当前图标位置
    float x = missionObject["x"].asFloat();
    float y = missionObject["y"].asFloat();
    // 更新支线任务图标的位置和可见性
    auto missionIcon = expandedMiniMapNode->getChildByName<cocos2d::Sprite*>(missionName);
    if (missionIcon) {
        missionIcon->setVisible(isVisible);
        if (isVisible) {
            missionIcon->setPosition(cocos2d::Vec2(x, y));
        }
    }
    else if (isVisible) {
        // 如果图标不存在且需要显示，则创建并添加到小地图2中
        missionIcon = cocos2d::Sprite::create("side_mission_icon.png");
        missionIcon->setName(missionName);
        missionIcon->setPosition(cocos2d::Vec2(x, y));
        expandedMiniMapNode->addChild(missionIcon);
    }
}
cocos2d::TMXTiledMap* Hud::getMiniMapNode() const {
    return miniMapNode;
}
cocos2d::TMXTiledMap* Hud::getExpandedMiniMapNode() const {
    return expandedMiniMapNode;
}

void Hud::hideFogLayers(cocos2d::TMXTiledMap* map, int index) {
    std::string layerName = "fog_" + std::to_string(index);
    auto layer = map->getLayer(layerName);
    if (layer) {
        layer->setVisible(false);
    }
    float scale = 32.0f / 256.0f;
    auto tpAnchors = tpAnchor->gettpPointActivation();
    miniMapNode->removeChildByName("Anchor");
    for (auto& anchor : tpAnchors) {
        Vec2 pos = anchor.first;
        if (anchor.second) {
            auto sprite = cocos2d::Sprite::create("Icon/Anchor.jpg");
            sprite->setScale(0.5);
            miniMapNode->addChild(sprite);
            sprite->setName("Anchor");
            sprite->setPosition(pos.x * scale, pos.y * scale);
        }
        else {
            auto sprite = cocos2d::Sprite::create("Icon/Anchor.jpg");
            sprite->setScale(0.5);
            miniMapNode->addChild(sprite);
            sprite->setName("Anchor");
            sprite->setColor(cocos2d::Color3B(128, 128, 128));
            sprite->setPosition(pos.x * scale, pos.y * scale);
        }
    }
}

// 初始化小地图玩家图标
void Hud::initMiniMapPlayerIcon() {
    miniMapPlayerIcon = cocos2d::Sprite::create("player.png");
    miniMapPlayerIcon->setScale(0.5);
    miniMapNode->addChild(miniMapPlayerIcon);

    // 获取小地图的 Objects 层和 SpawnPoint 对象
    auto objectLayer = miniMapNode->getObjectGroup("Objects");
    auto spawnPoint = objectLayer->getObject("SpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();

    // 设置小地图玩家图标的位置
    miniMapPlayerIcon->setPosition(cocos2d::Vec2(x, y));

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

    // 更新小地图的位置
    updateMiniMapPosition();

    // 更新小地图玩家图标的位置
    float scale = 32.0f / 256.0f;
    auto playerPos = player->getPosition();
    miniMapPlayerIcon->setPosition(playerPos.x * scale, playerPos.y * scale);

    // 使小地图始终以玩家为中心
    miniMapNode->setPosition(cocos2d::Vec2(-playerPos.x * scale + 50, -playerPos.y * scale + 50));

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
    cocos2d::Action* cooldownAction = cocos2d::ProgressTo::create(2 * cdTime, 0);
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
        skillBarNode[i]->setPosition(cocos2d::Vec2(offsetX + 300 + 60 * i, offsetY));
        skillCooldownBars[i]->setPosition(cocos2d::Vec2(offsetX + 300 + 60 * i, offsetY));
    }
}

// 更新小地图的位置
void Hud::updateMiniMapPosition() {
    float offsetX = -180; // 距离 HUD 左侧的偏移量
    float offsetY = -720; // 距离 HUD 顶部的偏移量

    // 设置小地图的位置
    miniMapNode->setPosition(cocos2d::Vec2(offsetX, offsetY));

}

void Hud::toggleMiniMap() {
    

    if (isMiniMapExpanded) {

        player->getChildByName("sprite")->setVisible(true);
        // 关闭新加载的小地图
        if (expandedMiniMapNode) {
            expandedMiniMapNode->removeFromParent();
            expandedMiniMapNode = nullptr;
        }

        // 显示原有的小地图
        miniMapNode->setVisible(true);
        clipper->setVisible(true); // 显示裁剪节点
        borderNode->setVisible(true); // 显示边框节点
        isMiniMapExpanded = false;
    }
    else {
        player->getChildByName("sprite")->setVisible(false);
        // 隐藏原有的小地图
        miniMapNode->setVisible(false);
        clipper->setVisible(false); // 隐藏裁剪节点
        borderNode->setVisible(false); // 隐藏边框节点

        // 加载新的小地图
        expandedMiniMapNode = cocos2d::TMXTiledMap::create("/maps/small_map2.tmx");
        expandedMiniMapNode->setScale(0.6f);
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        expandedMiniMapNode->setPosition(cocos2d::Vec2(-300, -400));
        addChild(expandedMiniMapNode, 10);

        // 初始化新的小地图玩家图标
        auto expandedMiniMapPlayerIcon = cocos2d::Sprite::create("player.png");
        expandedMiniMapPlayerIcon->setScale(0.5);
        expandedMiniMapNode->addChild(expandedMiniMapPlayerIcon);

        // 获取小地图的 Objects 层和 SpawnPoint 对象
        auto objectLayer = expandedMiniMapNode->getObjectGroup("Objects");
        auto spawnPoint = objectLayer->getObject("SpawnPoint");
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();

        // 设置小地图玩家图标的位置
        expandedMiniMapPlayerIcon->setPosition(cocos2d::Vec2(x, y));

        // 同步玩家位置到新小地图中玩家的位置
        float scale = 32.0f / 256.0f;
        auto playerPos = player->getPosition();
        expandedMiniMapPlayerIcon->setPosition(playerPos.x * scale, playerPos.y * scale);
        auto tpAnchors = tpAnchor->gettpPointActivation();
        expandedMiniMapNode->removeChildByName("Anchor");
        for (auto& anchor : tpAnchors) {
            Vec2 pos = anchor.first;
            if (anchor.second) {
                auto button = cocos2d::MenuItemImage::create(
                    "Icon/Anchor.jpg",
                    "Icon/Anchor.jpg",
                    [this,pos](cocos2d::Ref* sender) {
                        player->setPosition(pos);
                        CCLOG(("touched!"));
                    });
                auto menu = cocos2d::Menu::create(button, nullptr);
                menu->setPosition(cocos2d::Vec2::ZERO);
                menu->setName("Anchor");
                expandedMiniMapNode->addChild(menu);
                button->setPosition(pos.x * scale, pos.y * scale);
            }
            else {
                auto sprite = cocos2d::Sprite::create("Icon/Anchor.jpg");
                expandedMiniMapNode->addChild(sprite);
                sprite->setName("Anchor");
                sprite->setColor(cocos2d::Color3B(128, 128, 128));
                sprite->setPosition(pos.x * scale, pos.y * scale);
            }
        }
        isMiniMapExpanded = true;
    }
}