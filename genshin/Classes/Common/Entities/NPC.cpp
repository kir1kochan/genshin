#include "cocos2d.h"

using namespace cocos2d;

class NPC : public Sprite {
public:
    NPC(const std::string& imageName);
    void setPosition(const Vec2& position);
    void startDialogue();
    static NPC* create(const std::string& imageName);

private:
    std::string dialogue;
};

NPC::NPC(const std::string& imageName) {
    // ʹ�� create() ��������ͼƬ
    if (!Sprite::initWithFile(imageName)) {
        CCLOG("Failed to load NPC image: %s", imageName.c_str());
    }
    dialogue = "I'm the princess left outside.\nBe my knight and be the strongest.\ntake me back to the castle and bring back my authority.";  // ��ʼ���Ի�����
}

void NPC::setPosition(const Vec2& position) {
    Sprite::setPosition(position);
}

void NPC::startDialogue() {
    // ���������ʾ NPC �ĶԻ�����ʱ����־���
    CCLOG("NPC Dialogue: %s", dialogue.c_str());

    // ��ʾ�Ի���
    auto label = Label::createWithTTF(dialogue, "fonts/Marker Felt.ttf", 24);  // �����ı�
    label->setPosition(this->getPosition() + Vec2(0, 50));  // �Ի�����ʾ�� NPC �Ϸ�
    this->getParent()->addChild(label, 3);  // ��ӶԻ��򵽳���
}

NPC* NPC::create(const std::string& imageName) {
    NPC* npc = new NPC(imageName);
    if (npc) {
        npc->autorelease();
        return npc;
    }
    return nullptr;
}
