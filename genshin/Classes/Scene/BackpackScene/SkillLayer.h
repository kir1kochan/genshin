#ifndef SKILL_LAYER_H
#define SKILL_LAYER_H

#include "cocos2d.h"

class SkillLayer : public cocos2d::Layer {
public:
    SkillLayer();
    virtual ~SkillLayer();

    // ��ʼ�����ܽ���
    virtual bool init();

    // ��������UI���
    void createSkillUI();

    // ����ʵ���ĺ�
    CREATE_FUNC(SkillLayer);
};

#endif // SKILL_LAYER_H
