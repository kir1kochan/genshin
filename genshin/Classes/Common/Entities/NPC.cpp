// NPC.cpp
#include "NPC.h"

USING_NS_CC;

NPC* NPC::create(const std::string& filename)
{
    NPC* npc = new (std::nothrow) NPC();
    if (npc && npc->initWithFile(filename))
    {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

void NPC::update(float dt)
{
    // 这里可以添加 NPC 的行为逻辑，比如巡逻、追踪等
}

void NPC::talk()
{
    // 显示对话
    CCLOG("NPC says: %s", dialogue.c_str());
}

void NPC::walk()
{
    // 行走动画逻辑
}
