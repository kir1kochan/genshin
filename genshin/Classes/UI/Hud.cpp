#include "Hud.h"
#include "Classes/Common/Entities/Player/Player.h"
#include <Scene/SceneObject/TPAnchor.h>

// ���캯������ʼ������Ա����
Hud::Hud(Player* player)
    : player(player), barWidth(200.0f), barHeight(20.0f), isMiniMapExpanded(false) {

    // ���� DrawNode ������ʾѪ����������
    healthBarNode = cocos2d::DrawNode::create();
    staminaBarNode = cocos2d::DrawNode::create();

    // ��Ѫ�����������ڵ����Ϊ�ӽڵ�
    addChild(healthBarNode, -1);
    addChild(staminaBarNode, -1);

    // ����С��ͼ�ڵ�
    miniMapNode = cocos2d::TMXTiledMap::create("/maps/small_map.tmx");
    miniMapNode->setScale(1.0f); // ����С��ͼ�����ű���

    // �����ü��ڵ�
    clipper = cocos2d::ClippingNode::create();
    clipper->setContentSize(cocos2d::Size(100, 100)); // ���òü������С
    clipper->setPosition(cocos2d::Vec2(-500, -20)); // ���òü�����λ��
    addChild(clipper, 1);

    // ����һ�����ֲ�
    stencil = cocos2d::DrawNode::create();
    stencil->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(100, 100), cocos2d::Color4F::WHITE);
    clipper->setStencil(stencil);

    // ��С��ͼ��ӵ��ü��ڵ���
    clipper->addChild(miniMapNode);

    // ����һ�� DrawNode ���ڻ��ƻ�ɫ�߿�
    borderNode = cocos2d::DrawNode::create();
    float borderWidth = 2.0f; // �߿���
    cocos2d::Vec2 rect[4] = {
        cocos2d::Vec2(-borderWidth, -borderWidth),
        cocos2d::Vec2(100 + borderWidth, -borderWidth),
        cocos2d::Vec2(100 + borderWidth, 100 + borderWidth),
        cocos2d::Vec2(-borderWidth, 100 + borderWidth)
    };
    borderNode->drawPolygon(rect, 4, cocos2d::Color4F(0,0,0,0), borderWidth, cocos2d::Color4F::YELLOW);
    borderNode->setPosition(clipper->getPosition()); // ���ñ߿�λ����ü��ڵ�һ��
    addChild(borderNode, 2); // ���߿�ڵ���ӵ� HUD ��

    // ��ʼ��С��ͼ���ͼ��
    initMiniMapPlayerIcon();

    // ���ó�ʼ HUD ��λ��
    setPosition(cocos2d::Vec2(0, 100));  // HUD �������ҵĶ���

    // ��ʼ�������������λ��
    updateBarPositions();

    // ��ʼ��С��ͼ��λ��
    updateMiniMapPosition();


}

// ��ʼ��С��ͼ���ͼ��
void Hud::initMiniMapPlayerIcon() {
    miniMapPlayerIcon = cocos2d::Sprite::create("player.png");
    miniMapNode->addChild(miniMapPlayerIcon);

    // ��ȡС��ͼ�� Objects ��� SpawnPoint ����
    auto objectLayer = miniMapNode->getObjectGroup("Objects");
    auto spawnPoint = objectLayer->getObject("SpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();

    // ����С��ͼ���ͼ���λ��
    miniMapPlayerIcon->setPosition(cocos2d::Vec2(x, y));

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

    // ����С��ͼ��λ��
    updateMiniMapPosition();

    // ����С��ͼ���ͼ���λ��
    float scale = 32.0f / 256.0f;
    auto playerPos = player->getPosition();
    miniMapPlayerIcon->setPosition(playerPos.x * scale, playerPos.y * scale);

    // ʹС��ͼʼ�������Ϊ����
    miniMapNode->setPosition(cocos2d::Vec2(-playerPos.x * scale + 50, -playerPos.y * scale + 50));

}

// ����Ѫ����ʾ
void Hud::updateHealthBar(float healthPercent) {
    drawProgressBar(healthBarNode, healthPercent, cocos2d::Color4F::RED);
}

// ������������ʾ
void Hud::updateStaminaBar(float staminaPercent) {
    drawProgressBar(staminaBarNode, staminaPercent, cocos2d::Color4F::GREEN);
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
}

// ����С��ͼ��λ��
void Hud::updateMiniMapPosition() {
    float offsetX = -180; // ���� HUD ����ƫ����
    float offsetY = -720; // ���� HUD ������ƫ����

    // ����С��ͼ��λ��
    miniMapNode->setPosition(cocos2d::Vec2(offsetX, offsetY));

}

void Hud::toggleMiniMap() {
    if (isMiniMapExpanded) {
        // �ر��¼��ص�С��ͼ
        if (expandedMiniMapNode) {
            expandedMiniMapNode->removeFromParent();
            expandedMiniMapNode = nullptr;
        }

        // ��ʾԭ�е�С��ͼ
        miniMapNode->setVisible(true);
        clipper->setVisible(true); // ��ʾ�ü��ڵ�
        borderNode->setVisible(true); // ��ʾ�߿�ڵ�
        isMiniMapExpanded = false;
    }
    else {
        // ����ԭ�е�С��ͼ
        miniMapNode->setVisible(false);
        clipper->setVisible(false); // ���زü��ڵ�
        borderNode->setVisible(false); // ���ر߿�ڵ�

        // �����µ�С��ͼ
        expandedMiniMapNode = cocos2d::TMXTiledMap::create("/maps/small_map2.tmx");
        expandedMiniMapNode->setScale(0.6f);
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        expandedMiniMapNode->setPosition(cocos2d::Vec2(-300, -400));
        addChild(expandedMiniMapNode, 10);

        // ��ʼ���µ�С��ͼ���ͼ��
        auto expandedMiniMapPlayerIcon = cocos2d::Sprite::create("player.png");
        expandedMiniMapNode->addChild(expandedMiniMapPlayerIcon);

        // ��ȡС��ͼ�� Objects ��� SpawnPoint ����
        auto objectLayer = expandedMiniMapNode->getObjectGroup("Objects");
        auto spawnPoint = objectLayer->getObject("SpawnPoint");
        float x = spawnPoint["x"].asFloat();
        float y = spawnPoint["y"].asFloat();

        // ����С��ͼ���ͼ���λ��
        expandedMiniMapPlayerIcon->setPosition(cocos2d::Vec2(x, y));

        // ͬ�����λ�õ���С��ͼ����ҵ�λ��
        float scale = 32.0f / 256.0f;
        auto playerPos = player->getPosition();
        expandedMiniMapPlayerIcon->setPosition(playerPos.x * scale, playerPos.y * scale);


        isMiniMapExpanded = true;
    }
}


void Hud::updateMissionIconPosition(int stage) {
    if (!expandedMiniMapNode) {
        return; // ���С��ͼ2δ���أ���ֱ�ӷ���
    }

    // ��ȡ mission ͼ��
    auto missionLayer = expandedMiniMapNode->getObjectGroup("mission");
    if (!missionLayer) {
        return; // ��� mission ͼ�㲻���ڣ���ֱ�ӷ���
    }

    // ��ȡ��������ͼ�����
    auto missionObject = missionLayer->getObject("MissionIcon");
    if (missionObject.empty()) {
        return; // �����������ͼ����󲻴��ڣ���ֱ�ӷ���
    }

    // ��ȡ��ǰͼ��λ��
    float x = missionObject["x"].asFloat();
    float y = missionObject["y"].asFloat();

    // ���ݽ׶�ֵ����ͼ��λ��
    switch (stage) {
    case 0:
        x -= 200;
        break;
    case 1:
        x += 300;
        y += 300;
        break;
        // ���Ը�����Ҫ��Ӹ���׶�
    default:
        break;
    }

    // ������������ͼ���λ��
    auto missionIcon = expandedMiniMapNode->getChildByName<cocos2d::Sprite*>("MissionIcon");
    if (missionIcon) {
        missionIcon->setPosition(cocos2d::Vec2(x, y));
    }
    else {
        // ���ͼ�겻���ڣ��򴴽�����ӵ�С��ͼ2��
        missionIcon = cocos2d::Sprite::create("mission_icon.png");
        missionIcon->setName("MissionIcon");
        missionIcon->setPosition(cocos2d::Vec2(x, y));
        expandedMiniMapNode->addChild(missionIcon);
    }
}

void Hud::updateSideMissionIconPosition(const std::string& missionName, bool isVisible) {
    if (!expandedMiniMapNode) {
        return; // ���С��ͼ2δ���أ���ֱ�ӷ���
    }

    // ��ȡ sideMission ͼ��
    auto sideMissionLayer = expandedMiniMapNode->getObjectGroup("sideMission");
    if (!sideMissionLayer) {
        return; // ��� sideMission ͼ�㲻���ڣ���ֱ�ӷ���
    }

    // ��ȡ֧������ͼ�����
    auto missionObject = sideMissionLayer->getObject(missionName);
    if (missionObject.empty()) {
        return; // ���֧������ͼ����󲻴��ڣ���ֱ�ӷ���
    }

    // ��ȡ��ǰͼ��λ��
    float x = missionObject["x"].asFloat();
    float y = missionObject["y"].asFloat();

    // ����֧������ͼ���λ�úͿɼ���
    auto missionIcon = expandedMiniMapNode->getChildByName<cocos2d::Sprite*>(missionName);
    if (missionIcon) {
        missionIcon->setVisible(isVisible);
        if (isVisible) {
            missionIcon->setPosition(cocos2d::Vec2(x, y));
        }
    }
    else if (isVisible) {
        // ���ͼ�겻��������Ҫ��ʾ���򴴽�����ӵ�С��ͼ2��
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

void Hud::hideFogLayers(cocos2d::TMXTiledMap* map) {
    for (int i = 1; i <= 5; ++i) {
        std::string layerName = "fog_" + std::to_string(i);
        auto layer = map->getLayer(layerName);
        if (layer) {
            layer->setVisible(false);
        }
    }
}