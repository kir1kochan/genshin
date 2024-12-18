#ifndef SKILL_LAYER_H
#define SKILL_LAYER_H

#include <iomanip>
#include "cocos2d.h"
#include "../../Common/Entities/Player/Player.h"
#include "../../Common/Entities/Player/Skill/Skill.h"
#include "BackpackMainLayer.h"

class SkillLayer : public cocos2d::Layer {
public:
    SkillLayer();
    virtual ~SkillLayer();

    // 初始化技能界面
    virtual bool init();

    // 更新显示
    void update();

    // 设定玩家指针
    void setPlayer(Player* player);


    void setSkillToUnload(int i);
    void setSkillToEquip(int i);
    void updateSkills();
    // 生成实例的宏
    CREATE_FUNC(SkillLayer);
private:
    // 玩家及其相关信息
    Player* player;
    std::vector<std::shared_ptr<Skill>> unlockedSkills;
    std::vector<std::shared_ptr<Skill>> skillBar;
    std::vector<std::shared_ptr<Skill>> availableSkills;

    // 用于操作技能装配
    int skillToUnload=-1;
    int skillToEquip=-1;

    // 创建技能UI组件
    void createSkillUI();

    // 从玩家类中加载技能相关列表
    void loadSkills();

    // 加载技能图片
    void loadSkillsIcon();
};

#endif // SKILL_LAYER_H
