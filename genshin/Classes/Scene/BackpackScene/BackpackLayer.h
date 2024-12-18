#ifndef BACKPACK_LAYER_H
#define BACKPACK_LAYER_H

#include "cocos2d.h"
#include "Common/Backpack/Backpack.h"
#include "Common/Entities/Player/Player.h"
#include "BackpackMainLayer.h"
#include "Common/Item/Equipment/Equipment.h"




class BackpackLayer : public cocos2d::Layer {
public:
    BackpackLayer();
    ~BackpackLayer();

    // ��ʼ����������
    bool init();

    // ����������Ʒͼ��
    void createItemIcon();

    void setPlayer(Player* player);

    // ����ʵ���ĺ�
    CREATE_FUNC(BackpackLayer);

    void createNums();

    // �����ض���Ʒ������
    void uploadNumById(int id);

    // ��ж�µ�װ�����ڵ�һ����λ
    void addItemToBlank(int id);

private:

    Player* player;
    Backpack* backpack;

    // ��¼����ͼ��չʾ���������
    std::unordered_map<int, int> placeToId;
};

#endif // BACKPACK_LAYER_H
