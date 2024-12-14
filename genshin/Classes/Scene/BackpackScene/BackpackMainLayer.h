#ifndef BACKPACK_MAIN_LAYER_H
#define BACKPACK_MAIN_LAYER_H

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "BackpackLayer.h"
#include "SkillLayer.h"
#include "EquipmentLayer.h"
#include "../../Common/Entities/Player/Player.h"

class BackpackMainLayer : public cocos2d::Layer {
public:
    BackpackMainLayer();
    virtual ~BackpackMainLayer();

    // ����������Layer�������������
    static BackpackMainLayer* create(Player* player);

    // ��ʼ��������Layer��UIԪ��
    virtual bool init();

    // ����ʵ���ĺ�
    CREATE_FUNC(BackpackMainLayer);

    // �������ָ��
    void setPlayer(Player* player);

    // ��ȡ���ָ��
    Player* getPlayer() const;

    // �л�����������
    void switchToBackpackLayer();

    // �л������ܽ���
    void switchToSkillLayer();

    // �л���װ������
    void switchToEquipmentLayer();

    void adjustSizeForTransition();
private:
    // ��Ҷ���
    Player* player;

    // ��������Layer
    BackpackLayer* backpackLayer;

    // ���ܽ���Layer
    SkillLayer* skillLayer;

    // װ������Layer
    EquipmentLayer* equipmentLayer;

    // �л�����һ�����水ť
    cocos2d::ui::Button* nextButton;

    // �л�����һ�����水ť
    cocos2d::ui::Button* prevButton;

    // �����������UI�����顢�ȼ��������ȣ�
    void updatePlayerData();

    // �������������ס���Ʒ���ӵ���ʾ
    void createEquipmentGrid();

    // �������������ӵ���ʾ
    void createSkillGrid();

    // ��������л�����ļ�ͷ
    void createArrowButtons();

    // �������������ʾ
    void createPlayerDataDisplay();
};

#endif // BACKPACK_MAIN_LAYER_H
