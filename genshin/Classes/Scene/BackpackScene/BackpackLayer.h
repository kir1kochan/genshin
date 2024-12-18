#ifndef BACKPACK_LAYER_H
#define BACKPACK_LAYER_H

#include "cocos2d.h"
#include "Common/Backpack/Backpack.h"
#include "Common/Entities/Player/Player.h"

class BackpackLayer : public cocos2d::Layer {
public:
    BackpackLayer();
    ~BackpackLayer();

    // 初始化背包界面
    bool init();

    // 创建背包物品图标
    void createItemIcon();

    void setPlayer(Player* player);

    // 生成实例的宏
    CREATE_FUNC(BackpackLayer);

private:

    Player* player;
    Backpack* backpack;

    // 存储图标和数量的关系
    std::unordered_map<cocos2d::Sprite, int> iconToNum;

};

#endif // BACKPACK_LAYER_H
