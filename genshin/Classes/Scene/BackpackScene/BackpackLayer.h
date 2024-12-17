#ifndef BACKPACK_LAYER_H
#define BACKPACK_LAYER_H

#include "cocos2d.h"

class BackpackLayer : public cocos2d::Layer {
public:
    BackpackLayer();
    virtual ~BackpackLayer();

    // 初始化背包界面
    virtual bool init();

    // 创建背包物品图标
    void createItemIcon();

    // 生成实例的宏
    CREATE_FUNC(BackpackLayer);
};

#endif // BACKPACK_LAYER_H
