#ifndef CREATOR_H
#define CREATOR_H

#include "cocos2d.h"
#include "Element.h"

class Creator : public cocos2d::Node {
protected:
    float health;          // ��ǰѪ��
    float maxHealth;       // ���Ѫ��
    Element element;       // Ԫ������

public:
    // ���캯��
    Creator(float health, Element element);

    // Ĭ�Ϲ��캯��
    Creator();

    // ����������
    virtual ~Creator();

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
    virtual void attack(Creator& target);

    // ��ӡ��ǰ״̬
    virtual void printStatus();
};

#endif // CREATOR_H
