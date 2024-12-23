#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "TestScene.h"

// 构造函数，初始化各成员变量
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f), isMiniMapExpanded(false) {
    // 创建 DrawNode 用于显示血条和体力条
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // 将血条和体力条节点添加为子节点
    addChild(healthBarNode, -1);
    addChild(staminaBarNode, -1);

    // 创建小地图节点
    miniMapNode = cocos2d::TMXTiledMap::create("/maps/small_map.tmx");
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
    borderNode->drawPolygon(rect, 4, cocos2d::Color4F(0,0,0,0), borderWidth, cocos2d::Color4F::YELLOW);
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


}

// 初始化小地图玩家图标
void Hud::initMiniMapPlayerIcon() {
    miniMapPlayerIcon = cocos2d::Sprite::create("player.png");
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

        isMiniMapExpanded = true;
    }
}