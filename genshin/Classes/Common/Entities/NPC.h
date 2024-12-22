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
    void talk(); // NPC �Ի�����
    void walk(); // NPC ���߶���

private:
    std::string name;
    std::string dialogue;
    // ��Ӹ���NPC���е����Ժͷ���
};

#endif // __NPC_H__
