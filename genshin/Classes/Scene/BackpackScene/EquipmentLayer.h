#ifndef EQUIPMENT_LAYER_H
#define EQUIPMENT_LAYER_H

#include "cocos2d.h"

class EquipmentLayer : public cocos2d::Layer {
public:
    EquipmentLayer();
    virtual ~EquipmentLayer();

    // ��ʼ��װ������
    virtual bool init();
    
    // ����װ��UI���
    void createEquipmentUI();

    // ����ʵ���ĺ�
    CREATE_FUNC(EquipmentLayer);
};

#endif // EQUIPMENT_LAYER_H
