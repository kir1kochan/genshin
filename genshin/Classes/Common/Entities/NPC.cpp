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
    // ���������� NPC ����Ϊ�߼�������Ѳ�ߡ�׷�ٵ�
}

void NPC::talk()
{
    // ��ʾ�Ի�
    CCLOG("NPC says: %s", dialogue.c_str());
}

void NPC::walk()
{
    // ���߶����߼�
}
