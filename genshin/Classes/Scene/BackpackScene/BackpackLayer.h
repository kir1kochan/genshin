#ifndef BACKPACK_LAYER_H
#define BACKPACK_LAYER_H

#include "cocos2d.h"

class BackpackLayer : public cocos2d::Layer {
public:
    BackpackLayer();
    virtual ~BackpackLayer();

    // ��ʼ����������
    virtual bool init();

    // ����������Ʒͼ��
    void createItemIcon();

    // ����ʵ���ĺ�
    CREATE_FUNC(BackpackLayer);
};

#endif // BACKPACK_LAYER_H
