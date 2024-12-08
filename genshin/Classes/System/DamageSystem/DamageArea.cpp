#include "DamageArea.h"
#include "cocos2d.h"
#include <cmath>

USING_NS_CC;

// 构造函数，初始化伤害区域
DamageArea::DamageArea(DamageAreaType type, const cocos2d::Vec2& pos, float dmg, Element dmgType, bool applyEffect,
    Player* player, // 传入玩家指针
    float rad, const cocos2d::Size& sz, const cocos2d::Vec2& start, const cocos2d::Vec2& end)
    : areaType(type), position(pos), damageAmount(dmg), damageType(dmgType), applyAttributesEffect(applyEffect),
    player(player), radius(rad), size(sz), startPoint(start), endPoint(end) {
}

// 判定是否与玩家发生碰撞并造成伤害
void DamageArea::checkDamage() {
    // 如果玩家对象为空，则不进行伤害计算
    if (!player) {
        return;
    }

    // 判断区域类型，并根据区域类型执行不同的碰撞检测
    switch (areaType) {
    case DamageAreaType::CIRCLE:
        if (position.distance(player->getPosition()) <= radius) {
            // 玩家在圆形区域内，造成伤害
            float finalDamage = applyAttributesEffect(player, damageAmount);
            player->takeDamage(finalDamage);
            triggerDamageAnimation(player);
        }
        break;

    case DamageAreaType::RECTANGLE:
        if (player->getPosition().x >= position.x - size.width / 2 &&
            player->getPosition().x <= position.x + size.width / 2 &&
            player->getPosition().y >= position.y - size.height / 2 &&
            player->getPosition().y <= position.y + size.height / 2) {
            // 玩家在矩形区域内，造成伤害
            float finalDamage = applyAttributesEffect(player, damageAmount);
            player->takeDamage(finalDamage);
            triggerDamageAnimation(player);
        }
        break;

    case DamageAreaType::LINE:
        if (distancePointToLine(player->getPosition(), startPoint, endPoint) <= radius) {
            // 玩家在线性区域内，造成伤害
            float finalDamage = applyAttributesEffect(player, damageAmount);
            player->takeDamage(finalDamage);
            triggerDamageAnimation(player);
        }
        break;

    default:
        break;
    }
}

// 计算点到直线的距离
float DamageArea::distancePointToLine(const cocos2d::Vec2& point, const cocos2d::Vec2& lineStart, const cocos2d::Vec2& lineEnd) {
    // 计算点到线段的最短距离（参考叉积公式）
    float A = point.y - lineStart.y;
    float B = lineStart.x - point.x;
    float C = point.x * lineStart.y - lineStart.x * point.y;

    float dist = std::abs(A * lineEnd.x + B * lineEnd.y + C) / std::sqrt(A * A + B * B);
    return dist;
}

// 根据属性调整伤害并附加效果
void DamageArea::applyAttributesEffect(Player* player, float baseDamage) {
    if (applyAttributesEffect) {
        float elementModifier = calculateElementalDamageModifier(damageType, player.getElement());
        player.takeDamage(elementModifier * baseDamage);
        player.applyElementalEffects(player, damageType)
    }

    player.takeDamage(elementModifier * baseDamage);
}
