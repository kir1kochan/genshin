#ifndef EQUIPMENT_LAYER_H
#define EQUIPMENT_LAYER_H

#include "cocos2d.h"

class EquipmentLayer : public cocos2d::Layer {
public:
    EquipmentLayer();
    ~EquipmentLayer();

    // 初始化装备界面
    bool init();
    
    // 创建装备UI组件
    void createEquipmentUI();

    // 生成实例的宏
    CREATE_FUNC(EquipmentLayer);
};

#endif // EQUIPMENT_LAYER_H
