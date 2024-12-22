#ifndef __TPANCHOR_H__
#define __TPANCHOR_H__

#include "cocos2d.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "../../Common/Entities/Player/Player.h"  

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

class TPAnchor : public cocos2d::Node {
public:
    TPAnchor();
    ~TPAnchor();

    // 通过 Player 的引用进行传送
    void teleport(Player& player);
    void setPlayer(Player* player) {
        this->player = player;
    }
    // 设置某个 TP 点位为激活
    void activateTPPoint(const cocos2d::Vec2& point);

    // 从 JSON 文件加载 TP 点位数据
    void loadFromJson(const std::string& jsonFilePath);

    // 保存 TP 点位数据到 JSON 文件
    void saveToJson(const std::string& jsonFilePath);


    std::unordered_map<cocos2d::Vec2, bool>  gettpPointActivation() const;

private:
    // 存储所有 TP 点位坐标
    std::vector<cocos2d::Vec2> tpPoints;

    // 存储每个 TP 点位的激活情况，true 表示激活，false 表示未激活
    std::unordered_map<cocos2d::Vec2, bool> tpPointActivation;

    std::unordered_map<cocos2d::Vec2,int> tpPointsIDs;

    Player* player;
};

#endif  // __TPANCHOR_H__
