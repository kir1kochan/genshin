#include "BackpackLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

BackpackLayer::BackpackLayer() {}

BackpackLayer::~BackpackLayer() {}

bool BackpackLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // �������������UI���
    createBackpackUI();

    return true;
}

void BackpackLayer::createBackpackUI() {
    // ����һ����ǩ��ʾ��������ı���
    auto label = Label::createWithSystemFont("Backpack", "Arial", 24);
    label->setPosition(Vec2(1200, 1050));
    this->addChild(label);

    // ��Ʒ���ӵ���ʼλ��
    float startX = 650.0f;
    float startY = 960.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 13; 
    float itemWidth = 90.0f;
    float itemHeight = 90.0f;

    // ������ƽ�̸���
    for (int i = 0; i < 130; ++i) { // ������Ҫ���� 20 ������
        // ���㵱ǰ���ӵ�λ��
        float x = startX + (i % itemsPerRow) * (itemWidth + 5); // ÿ������֮���� 5 �ļ��
        float y = startY - (i / itemsPerRow) * (itemHeight + 5); // ÿ�еĸ��Ӽ�� 5

        // �������ӵ� Sprite ������λ��
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        this->addChild(itemSprite);
    }
}

