#ifndef __TPANCHOR_H__
#define __TPANCHOR_H__

#include "cocos2d.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Player.h"  

class TPAnchor : public cocos2d::Node {
public:
    TPAnchor();
    ~TPAnchor();

    // 通过 Player 的引用进行传送
    void teleport(Player& player);

    // 设置某个 TP 点位为激活
    void activateTPPoint(const cocos2d::Vec2& point);

    // 从 JSON 文件加载 TP 点位数据
    void loadFromJson(const std::string& jsonFilePath);

    // 保存 TP 点位数据到 JSON 文件
    void saveToJson(const std::string& jsonFilePath);

private:
    // 存储所有 TP 点位坐标
    std::vector<cocos2d::Vec2> tpPoints;

    // 存储每个 TP 点位的激活情况，true 表示激活，false 表示未激活
    std::unordered_map<cocos2d::Vec2, bool> tpPointActivation;

};

#endif  // __TPANCHOR_H__
