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

    // 初始化背包界面
    bool init();

    // 创建背包物品图标
    void createItemIcon();

    void setPlayer(Player* player);

    // 生成实例的宏
    CREATE_FUNC(BackpackLayer);

    void createNums();

    // 更新特定物品的数量
    void uploadNumById(int id);

    // 将卸下的装备加在第一个空位
    void addItemToBlank(int id);

private:

    Player* player;
    Backpack* backpack;

    // 记录管理图标展示情况的数组
    std::unordered_map<int, int> placeToId;
};

#endif // BACKPACK_LAYER_H
