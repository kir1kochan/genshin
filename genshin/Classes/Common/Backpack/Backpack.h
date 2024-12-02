#ifndef BACKPACK_H
#define BACKPACK_H

#include <unordered_map>
#include <string>
#include "Item.h"
#include "Player.h"

class Backpack {
public:

    // 构造函数
    Backpack(const std::vector<Item*>& itemPointers);
   
    // 通过物品指针添加物品
    void addItem(Item* item, int count = 1);

    // 通过物品ID添加物品
    void addItemById(int itemId, int count = 1);

    // 通过物品指针移除物品
    void removeItem(Item* item, int count = 1);

    // 通过物品ID移除物品
    void removeItemById(int itemId, int count = 1);

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

    friend:class Player;
private:
    std::unordered_map<Item*, int> items;  // 物品指针和数量的映射
    std::unordered_map<int, Item*> idToItemMap;  // 物品ID到物品指针的映射
};

#endif // BACKPACK_H
