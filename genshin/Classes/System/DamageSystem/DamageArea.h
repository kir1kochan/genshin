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
    DamageAreaType areaType;               // �������ͣ�Բ�Ρ����Ρ�ֱ�ߣ�
    cocos2d::Vec2 position;                // ���������λ��
    float radius;                          // �뾶��Բ������ʹ�ã�
    cocos2d::Size size;                    // ��������Ĵ�С
    cocos2d::Vec2 startPoint, endPoint;    // ֱ������������յ�
    float damageAmount;                    // �˺�ֵ
    Element damageType;                    // �˺�����
    bool applyAttributesEffect;            // �Ƿ����Ԫ�����Ե����˺�
    Player* player;                        // ���ָ��
    float delay;                           // �ӳ�ʱ��
    float elapsed;                         // ����ʱ��
    // ��������Ӷ���id
public:
    // ���캯������ʼ���˺�����
    DamageArea(DamageAreaType type, const cocos2d::Vec2& pos, float dmg, Element dmgType, bool applyEffect,
        Player* player, // �������ָ��
        float rad = 0, const cocos2d::Size& sz = cocos2d::Size::ZERO,
        const cocos2d::Vec2& start = cocos2d::Vec2::ZERO, const cocos2d::Vec2& end = cocos2d::Vec2::ZERO, float delayTime);

    // �ж��Ƿ���Ŀ�귢����ײ������˺�
    void checkDamage();

private:
    // ����㵽ֱ�ߵľ���
    float distancePointToLine(const cocos2d::Vec2& point, const cocos2d::Vec2& lineStart, const cocos2d::Vec2& lineEnd);
};

#endif // DAMAGEAREA_H
