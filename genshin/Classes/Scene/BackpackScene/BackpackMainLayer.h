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

    cocos2d::Node* equipmentIconsContainer; // �洢װ��ͼ�������

    cocos2d::Node* playerInfoContainer; // �洢װ��ͼ�������

    cocos2d::Node* skillIconsContainer; // �洢װ��ͼ�������

    Label* _hoverLabel = nullptr;  // ������ʾװ���ŵ�װ���ͼ��������ı�ǩ

    Layer* _hoverLabelBackground = nullptr; // ��ǩ�ı���

    // ������ײ��
    void createBoundingBoxForIcons(cocos2d::Sprite* sprite);

    // �����������UI�����顢�ȼ��������ȣ�
    void updatePlayerData();

    // �������������ס���Ʒ���ӵ���ʾ
    void createEquipmentGrid();

    // �������������ӵ���ʾ
    void createSkillGrid();

    // �����������ӵ���ʾ
    void createBackpackUI();

    // ��������л�����ļ�ͷ
    void createArrowButtons();

    // �������������ʾ
    void createPlayerDataDisplay();

    // ����װ��ͼ��
    void createEquipmentIcons();

    // ˫���¼�������
    void addDoubleClickListener(cocos2d::Sprite* target, const std::function<void()>& callback);

    // ˢ��װ��ͼ��
    void refreshEquipmentIcons();

    // ���ݰ�ť�л���ɫ���Ժ����
    void onLeftArrowClicked();
    void onRightArrowClicked();

    // �����ͣ������
    void addHoverListenerForIcons(Sprite* icon, const std::string& name, const std::string& effectValue, int id);

    // װ����Ϣ��ʾ���ͼ�����Ϣ��ʾ��
    void showHoverInfo(const std::string& name, const std::string& effectValue, const Vec2& position);
    // ������ͣ��Ϣ
    void hideHoverInfo()
};

#endif // BACKPACK_MAIN_LAYER_H
