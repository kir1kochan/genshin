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
    label->setPosition(Vec2(300, 400));
    this->addChild(label);

    // ������Ը�����������������UIԪ�أ��缼�ܰ�ť������ͼ���
}
