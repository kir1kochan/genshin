#include "EquipmentLayer.h"
#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

EquipmentLayer::EquipmentLayer() {}

EquipmentLayer::~EquipmentLayer() {}

bool EquipmentLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // ����װ�������UI���
    createEquipmentUI();

    return true;
}

void EquipmentLayer::createEquipmentUI() {
    // ����һ����ǩ��ʾװ������ı���
    auto label = Label::createWithSystemFont("Equipment", "Arial", 24);
    label->setPosition(Vec2(300, 400));
    this->addChild(label);

    // ������Ը�����������������UIԪ�أ���װ���ۡ����ܰ�ť��
}
