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
    label->setPosition(Vec2(300, 400));
    this->addChild(label);

    // ������Ը�����������������UIԪ�أ�����Ʒ���ӡ���Ʒͼ���
}
