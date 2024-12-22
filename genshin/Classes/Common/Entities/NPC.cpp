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
    // 使用 create() 方法加载图片
    if (!Sprite::initWithFile(imageName)) {
        CCLOG("Failed to load NPC image: %s", imageName.c_str());
    }
    dialogue = "I'm the princess left outside.\nBe my knight and be the strongest.\ntake me back to the castle and bring back my authority.";  // 初始化对话内容
}

void NPC::setPosition(const Vec2& position) {
    Sprite::setPosition(position);
}

void NPC::startDialogue() {
    // 这里可以显示 NPC 的对话框，暂时用日志输出
    CCLOG("NPC Dialogue: %s", dialogue.c_str());

    // 显示对话框
    auto label = Label::createWithTTF(dialogue, "fonts/Marker Felt.ttf", 24);  // 创建文本
    label->setPosition(this->getPosition() + Vec2(0, 50));  // 对话框显示在 NPC 上方
    this->getParent()->addChild(label, 3);  // 添加对话框到场景
}

NPC* NPC::create(const std::string& imageName) {
    NPC* npc = new NPC(imageName);
    if (npc) {
        npc->autorelease();
        return npc;
    }
    return nullptr;
}
