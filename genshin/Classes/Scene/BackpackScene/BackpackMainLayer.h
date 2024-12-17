#ifndef BACKPACK_MAIN_LAYER_H
#define BACKPACK_MAIN_LAYER_H

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "BackpackLayer.h"
#include "EquipmentLayer.h"
#include "../../Common/Entities/Player/Player.h"

class SkillLayer;

struct HoverInfo {
    bool isHovering;             // �Ƿ�������ͣ
    std::string name;            // ͼ������
    std::string effectValue;     // Ч��
    std::function<void()> callback = nullptr;  // ��ѡ�Ļص�����
};


namespace std {
    // Ϊ cocos2d::Rect ����һ����ϣ����
    template <>
    struct hash<cocos2d::Rect> {
        size_t operator()(const cocos2d::Rect& rect) const {
            size_t h1 = std::hash<float>{}(rect.origin.x);
            size_t h2 = std::hash<float>{}(rect.origin.y);
            size_t h3 = std::hash<float>{}(rect.size.width);
            size_t h4 = std::hash<float>{}(rect.size.height);

            // ʹ����Ϲ�ϣ����
            return h1 ^ h2 ^ h3 ^ h4;
        }
    };

    // Ϊ cocos2d::Rect ������ȱȽϲ�����
    template <>
    struct equal_to<cocos2d::Rect> {
        bool operator()(const cocos2d::Rect& lhs, const cocos2d::Rect& rhs) const {
            return lhs.origin.x == rhs.origin.x &&
                lhs.origin.y == rhs.origin.y &&
                lhs.size.width == rhs.size.width &&
                lhs.size.height == rhs.size.height;
        }
    };
}

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

    // �����ͣ������
    void addHoverListenerForIcons(Sprite* icon, const std::string& name, const std::string& effectValue, int id, std::function<void()> cb = nullptr);
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

    // �ֳ��ĸ����������ͼ��ÿ������ͼ�洢��Ӧ����� Rect �� HoverInfo
    std::unordered_map<cocos2d::Rect, HoverInfo> topLeftGrid;     // ��������
    std::unordered_map<cocos2d::Rect, HoverInfo> topRightGrid;    // ��������
    std::unordered_map<cocos2d::Rect, HoverInfo> bottomLeftGrid;  // ��������
    std::unordered_map<cocos2d::Rect, HoverInfo> bottomRightGrid; // ��������
    int mouseArea; // ����ָʾ������ĸ�����

    float gaptime = 0.0f; // ���ڶ�ʱ����ʱ

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

    // װ����Ϣ��ʾ���ͼ�����Ϣ��ʾ��
    void showHoverInfo(const std::string& name, const std::string& effectValue, const Vec2& position);

    // ������ͣ��Ϣ
    void hideHoverInfo();

    void update(float deltatime);

    // ��������ͣ����
    void addHoverListener();
};

#endif // BACKPACK_MAIN_LAYER_H
