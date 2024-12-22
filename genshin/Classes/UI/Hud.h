#ifndef HUD_H
#define HUD_H

#include "cocos2d.h"
#include<vector>
class TPAnchor;
class Player;

class Hud : public cocos2d::Node {
private:
    // ���������ƽڵ�
    cocos2d::DrawNode* healthBarNode;  // Ѫ���ڵ�
    cocos2d::DrawNode* staminaBarNode; // �������ڵ�
    cocos2d::TMXTiledMap* miniMapNode; // С��ͼ�ڵ�
    cocos2d::Sprite* miniMapPlayerIcon; // С��ͼ���ͼ��
    cocos2d::ClippingNode* clipper;//�ü�
    cocos2d::DrawNode* stencil;//���ֲ�
    cocos2d::DrawNode* borderNode;//�߿�
    cocos2d::TMXTiledMap* expandedMiniMapNode;
    cocos2d::Sprite* expandedMiniMapPlayerIcon;
    // ���������ƽڵ�
    std::vector<cocos2d::DrawNode*> skillBarNode;   // �������ڵ�

    // ��ȴ���ӣ���������
    std::vector<cocos2d::ProgressTimer*> skillCooldownBars;  // ��ȴ������

    // �������Ŀ�Ⱥ͸߶�
    float barWidth;  // ���������ܿ��
    float barHeight; // �������ĸ߶�
    TPAnchor* tpAnchor;
    // ������Ҷ���
    Player* player;

    // С��ͼ��ʾ״̬
    bool isMiniMapExpanded;

    // ���½�����λ�ã������ص���
    void updateBarPositions();

    // ����ͨ�õľ��ν�����
    void drawProgressBar(cocos2d::DrawNode* barNode, float percent, cocos2d::Color4F color);

    // ��ʼ��С��ͼ���ͼ��
    void initMiniMapPlayerIcon();
public:
    // ���캯������������
    Hud(Player* player);
    virtual ~Hud();

    void setTPAnchor(TPAnchor* tpAnchor);
    // ��̬��������
    static Hud* create(Player* player);

    // ÿ֡���·���
    void update(float deltaTime);

    // ����Ѫ����������
    void updateHealthBar(float healthPercent);
    void updateStaminaBar(float staminaPercent);

    // װ������
    void equipSkill(int skillSlot, std::string skillName);
    // ж�ؼ���
    void unequipSkill(int skillSlot);

    void useSkill(int skillSlot, float cdTime);

    // ����С��ͼ��λ��
    void updateMiniMapPosition();
    //�л�С��ͼ��ʾ��ʽ
    void toggleMiniMap();

    // ������������ͼ��λ��
    void updateMissionIconPosition(int stage);
    //֧��
    void updateSideMissionIconPosition(const std::string& missionName, bool isVisible);
    //������
    void hideFogLayers(cocos2d::TMXTiledMap* map);
    //������
    void hideFogLayers(cocos2d::TMXTiledMap* map, int index);
    cocos2d::TMXTiledMap* getMiniMapNode() const;
    cocos2d::TMXTiledMap* getExpandedMiniMapNode() const;

    bool getIsExpanded() {
        return isMiniMapExpanded;
    }
};

#endif // HUD_H