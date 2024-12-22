#ifndef NPC_H
#define NPC_H

#include "cocos2d.h"

class NPC : public cocos2d::Sprite {
public:
    NPC(const std::string& imageName);
    void setPosition(const cocos2d::Vec2& position);
    void startDialogue();
    static NPC* create(const std::string& imageName);

private:
    std::string dialogue;  // NPC 对话文本
};

#endif // NPC_H
