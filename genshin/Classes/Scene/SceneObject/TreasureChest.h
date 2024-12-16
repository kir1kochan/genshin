#ifndef __TREASURECHEST_H__
#define __TREASURECHEST_H__

#include "cocos2d.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "../../Common/Backpack/Backpack.h"  


// 自定义哈希函数
namespace std {
    template <>
    struct hash<cocos2d::Vec2> {
        size_t operator()(const cocos2d::Vec2& vec) const {
            // 简单哈希：将 x 和 y 转换为整数并混合
            return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
        }
    };
}

class TreasureChest : public cocos2d::Node {
public:
    TreasureChest();
    ~TreasureChest();

    // 设置宝箱内的物品 ID
    void setItemInChest(const cocos2d::Vec2& chestPosition, int itemID);

    // 开启宝箱
    void openChest(Backpack& backpack, const cocos2d::Vec2& chestPosition);

    // 从 JSON 文件加载宝箱数据
    void loadFromJson(const std::string& jsonFilePath);

    // 保存宝箱数据到 JSON 文件
    void saveToJson(const std::string& jsonFilePath);

private:
    // 存储所有宝箱的坐标
    std::vector<cocos2d::Vec2> chestPositions;

    // 存储每个宝箱的物品 ID，0 表示宝箱为空
    std::unordered_map<cocos2d::Vec2, int> chestContents;
};

#endif  // __TREASURECHEST_H__
