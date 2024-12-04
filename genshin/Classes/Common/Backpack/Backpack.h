#ifndef BACKPACK_H
#define BACKPACK_H

#include <unordered_map>
#include <string>
#include "../Item/Item.h"
#include "../Entities/Player/Player.h"
#include<vector>

// 前向声明类
class Item;
class Player;
class Backpack {
public:

    // 构造函数
    Backpack(const std::vector<Item*>* itemPointers);
   
    // 通过物品指针添加物品
    void addItem(Item* item, int count = 1);

    // 通过物品ID添加物品
    void addItem(int itemId, int count = 1);

    // 通过物品指针移除物品
    void removeItem(Item* item, int count = 1);

    // 通过物品ID移除物品
    void removeItem(int itemId, int count = 1);

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
    Item* createItemById(int id, const std::string& jsonString);

    friend  class Player;
private:
    std::unordered_map<Item*, int> items;  // 物品指针和数量的映射
    std::unordered_map<int, Item*> idToItemMap;  // 物品ID到物品指针的映射
};

#endif // BACKPACK_H
