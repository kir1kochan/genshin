#ifndef DAMAGEAREA_H
#define DAMAGEAREA_H

#include "../Common/Entities/Player/Player.h"
#include "../Common/Element/Element.h"
#include "cocos2d.h"

enum class DamageAreaType {
    CIRCLE,
    RECTANGLE,
    LINE
};

class DamageArea : public cocos2d::Node {
private:
    DamageAreaType areaType;               // 区域类型（圆形、矩形、直线）
    cocos2d::Vec2 position;                // 区域的中心位置
    float radius;                          // 半径（圆形区域使用）
    cocos2d::Size size;                    // 矩形区域的大小
    cocos2d::Vec2 startPoint, endPoint;    // 直线区域的起点和终点
    float damageAmount;                    // 伤害值
    Element damageType;                    // 伤害类型
    bool applyAttributesEffect;            // 是否根据元素属性调整伤害
    Player* player;                        // 玩家指针
    float delay;                           // 延迟时间
    float elapsed;                         // 已用时间
    // 还可以添加动画id
public:
    // 构造函数，初始化伤害区域
    DamageArea(DamageAreaType type, const cocos2d::Vec2& pos, float dmg, Element dmgType, bool applyEffect,
        Player* player, // 传入玩家指针
        float rad = 0, const cocos2d::Size& sz = cocos2d::Size::ZERO,
        const cocos2d::Vec2& start = cocos2d::Vec2::ZERO, const cocos2d::Vec2& end = cocos2d::Vec2::ZERO, float delayTime);

    // 判定是否与目标发生碰撞并造成伤害
    void checkDamage();

private:
    // 计算点到直线的距离
    float distancePointToLine(const cocos2d::Vec2& point, const cocos2d::Vec2& lineStart, const cocos2d::Vec2& lineEnd);
};

#endif // DAMAGEAREA_H
