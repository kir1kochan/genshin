#ifndef BACKPACK_LAYER_H
#define BACKPACK_LAYER_H

#include "cocos2d.h"
#include "Common/Backpack/Backpack.h"
#include "Common/Entities/Player/Player.h"

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

private:

    Player* player;
    Backpack* backpack;

    // �洢ͼ��������Ĺ�ϵ
    std::unordered_map<cocos2d::Sprite, int> iconToNum;

};

#endif // BACKPACK_LAYER_H
