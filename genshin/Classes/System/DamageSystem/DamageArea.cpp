#include "DamageArea.h"
#include "cocos2d.h"
#include <cmath>

USING_NS_CC;

// ���캯������ʼ���˺�����
DamageArea::DamageArea(DamageAreaType type, const cocos2d::Vec2& pos, float dmg, Element dmgType, bool applyEffect,
    Player* player, // �������ָ��
    float rad, const cocos2d::Size& sz, const cocos2d::Vec2& start, const cocos2d::Vec2& end, float delayTime)
    : areaType(type), position(pos), damageAmount(dmg), damageType(dmgType), applyAttributesEffect(applyEffect),
    player(player), radius(rad), size(sz), startPoint(start), endPoint(end), delay(delayTime), elapsed(0.0f) {
}

// �ж��Ƿ�����ҷ�����ײ������˺�
void DamageArea::checkDamage(float deltaTime) {
    elapsed += deltaTime; // ��������ʱ��

    if (elapsed < delay) {
        return; // ���δ���ӳ�ʱ�䣬ֱ�ӷ���
    }

    if (!player) {
        return; // �����Ҷ���Ϊ�գ�������
    }

    switch (areaType) {
    case DamageAreaType::CIRCLE:
        if (position.distance(player->getPosition()) <= radius) {
            float finalDamage = applyAttributesEffect(player, damageAmount);
            player->takeDamage(finalDamage);
        }
        break;

    case DamageAreaType::RECTANGLE:
        if (player->getPosition().x >= position.x - size.width / 2 &&
            player->getPosition().x <= position.x + size.width / 2 &&
            player->getPosition().y >= position.y - size.height / 2 &&
            player->getPosition().y <= position.y + size.height / 2) {
            float finalDamage = applyAttributesEffect(player, damageAmount);
            player->takeDamage(finalDamage);
        }
        break;

    case DamageAreaType::LINE:
        if (distancePointToLine(player->getPosition(), startPoint, endPoint) <= radius) {
            float finalDamage = applyAttributesEffect(player, damageAmount);
            player->takeDamage(finalDamage);
        }
        break;

    default:
        break;
    }
}


// ����㵽ֱ�ߵľ���
float DamageArea::distancePointToLine(const cocos2d::Vec2& point, const cocos2d::Vec2& lineStart, const cocos2d::Vec2& lineEnd) {
    // ����㵽�߶ε���̾��루�ο������ʽ��
    float A = point.y - lineStart.y;
    float B = lineStart.x - point.x;
    float C = point.x * lineStart.y - lineStart.x * point.y;

    float dist = std::abs(A * lineEnd.x + B * lineEnd.y + C) / std::sqrt(A * A + B * B);
    return dist;
}

// �������Ե����˺�������Ч��
float DamageArea::applyAttributesEffect(Player* player, float baseDamage) {
    if (applyAttributesEffect) {
        float elementModifier = calculateElementalDamageModifier(damageType, player.getElement());
        baseDamage=elementModifier * baseDamage;
        player.applyElementalEffects(player, damageType)
    }

    return baseDamage;
}
