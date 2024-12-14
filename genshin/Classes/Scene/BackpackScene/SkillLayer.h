#ifndef SKILL_LAYER_H
#define SKILL_LAYER_H

#include "cocos2d.h"

class SkillLayer : public cocos2d::Layer {
public:
    SkillLayer();
    virtual ~SkillLayer();

    // 初始化技能界面
    virtual bool init();

    // 创建技能UI组件
    void createSkillUI();

    // 生成实例的宏
    CREATE_FUNC(SkillLayer);
};

#endif // SKILL_LAYER_H
