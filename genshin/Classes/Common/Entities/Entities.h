#ifndef ENTITIES_H
#define ENTITIES_H

#include "cocos2d.h"
#include "../Element/Element.h"

class Entities : public cocos2d::Node {
protected:
    float health;          // ��ǰѪ��
    float maxHealth;       // ���Ѫ��
    Element element;       // Ԫ������

public:
    // ���캯��
    Entities(float health, Element element);

    // Ĭ�Ϲ��캯��
    Entities();

    // ����������
    virtual ~Entities();

    // ��ȡ��ǰѪ��
    float getHealth() const;

    // ��ȡ���Ѫ��
    float getMaxHealth() const;

    // ��ȡԪ������
    Element getElement() const;

    // �ܵ��˺�
    virtual void takeDamage(float amount);

    // �ָ�Ѫ��
    virtual void heal(float amount);

    // ����Ŀ��
    virtual void attack(Entities& target);

    // ��ӡ��ǰ״̬
    virtual void printStatus();
};

#endif // ENTITIES_H
