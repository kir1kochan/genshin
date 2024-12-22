#pragma once
// NPC.h
#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"

class NPC : public cocos2d::Sprite
{
public:
    static NPC* create(const std::string& filename);

    void update(float dt);
    void talk(); // NPC 对话功能
    void walk(); // NPC 行走动作

private:
    std::string name;
    std::string dialogue;
    // 添加更多NPC特有的属性和方法
};

#endif // __NPC_H__
