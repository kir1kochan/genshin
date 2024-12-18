#ifndef BACKPACK_H
#define BACKPACK_H

#include <unordered_map>
#include <string>
#include "../Item/Item.h"
//#include "../Entities/Player/Player.h"
#include<vector>
#include "Classes/Common/Item/Equipment/Weapon/Weapon.h"
#include "Classes/Common/Item/Equipment/Armor/Armor.h"
#include "Classes/Common/Item/Equipment/Accessory/Accessory.h"
#include "Classes/Common/Item/Food/Food.h"
#include "Classes/Common/Item/Potion/Potion.h"
#include <memory>
// 前向声明类
class Item;
class Potion;
class Food;
class Weapon;
class Armor;
class Accessory;
class Player;

class Backpack {
public:
    // 默认构造函数
    Backpack();

    // 构造函数
    Backpack(const std::vector<std::shared_ptr<Item>>& itemPointers);

    // 通过物品指针添加物品
    void addItem(const std::shared_ptr<Item>& item, int count = 1);

    // 通过物品ID添加物品
    void addItem(int itemId, int count = 1);

    // 通过物品指针移除物品
    void removeItem(const std::shared_ptr<Item>& item, int count = 1);

    // 通过物品ID移除物品
    void removeItem(int itemId, int count = 1);

    // 根据id获得实例
    std::shared_ptr<Item> findItemById(int id);
    
    // 打印背包信息
    void printInfo() const;

    // 将背包数据序列化为 JSON 字符串
    std::string saveToJson() const;

    // 从 JSON 字符串加载背包数据
    void loadFromJson(const std::string& jsonString);

    // 将背包保存到文件
    void saveToFile(const std::string& filePath) const;

    // 从文件加载背包数据
    void loadFromFile(const std::string& filePath);

    // 解析 ID
    // 后续改进意见，可以根据游戏，设计更加完备的编码体系，可以以宏定义的方式出现
    // 解析 ID 并返回对应的物品类型
    std::shared_ptr<Item> createItemById(int id, const std::string& jsonString);

    // 根据物品ID获取物品数量
    int getItemCountById(int itemId) const;

    // 发送广播，标识背包食物相关有变动
    void sendFoodBroadcast();

    // 获取金币数量
    int getCoins() const;

    // 增加金币
    void addCoins(int amount);

    // 使用金币，成功返回true，失败返回false（金币不足）
    bool useCoins(int amount);

    const std::unordered_map<std::shared_ptr<Item>, int>& getItems() const;

    friend class Player;
private:
    std::unordered_map<std::shared_ptr<Item>, int> items;  // 物品指针和数量的映射
    std::unordered_map<int, std::shared_ptr<Item>> idToItemMap;  // 物品ID到物品指针的映射
    int coins;  // 存储金币数量
};

#endif // BACKPACK_H
