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
    float startX = 120.0f;
    float startY = 185.0f;

    // ÿ����ʾ���ٸ�����
    int itemsPerRow = 5;
    float itemWidth = 90.0f;
    float itemHeight = 90.0f;

    // ������ƽ�̸���
    for (int i = 0; i < 5; ++i) { // ������Ҫ���� 20 ������
        // ���㵱ǰ���ӵ�λ��
        float x = startX + (i % itemsPerRow) * (itemWidth + 5); // ÿ������֮���� 5 �ļ��
        float y = startY - (i / itemsPerRow) * (itemHeight + 5); // ÿ�еĸ��Ӽ�� 5

        // �������ӵ� Sprite ������λ��
        auto itemSprite = Sprite::create("UI/item_slot.png");
        itemSprite->setPosition(Vec2(x, y));
        this->addChild(itemSprite);
    }
    // ������Ը�����������������UIԪ�أ��缼�ܰ�ť������ͼ���
}
