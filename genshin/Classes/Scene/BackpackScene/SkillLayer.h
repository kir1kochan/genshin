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

    // ��ʼ�����ܽ���
    virtual bool init();

    // ������ʾ
    void update();

    // �趨���ָ��
    void setPlayer(Player* player);


    void setSkillToUnload(int i);
    void setSkillToEquip(int i);
    void updateSkills();
    // ����ʵ���ĺ�
    CREATE_FUNC(SkillLayer);
private:
    // ��Ҽ��������Ϣ
    Player* player;
    std::vector<std::shared_ptr<Skill>> unlockedSkills;
    std::vector<std::shared_ptr<Skill>> skillBar;
    std::vector<std::shared_ptr<Skill>> availableSkills;

    // ���ڲ�������װ��
    int skillToUnload=-1;
    int skillToEquip=-1;

    // ��������UI���
    void createSkillUI();

    // ��������м��ؼ�������б�
    void loadSkills();

    // ���ؼ���ͼƬ
    void loadSkillsIcon();
};

#endif // SKILL_LAYER_H
