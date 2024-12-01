#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>
#include "Item.h"

class Backpack {
public:
    // 添加物品
    void addItem(Item* item);

    // 移除物品
    void removeItem(int itemId);

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

private:
    std::vector<Item*> items; // 背包中的物品集合
};

#endif // BACKPACK_H
