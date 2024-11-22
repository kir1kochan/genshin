#ifndef SPIRIT_H
#define SPIRIT_H

#include "cocos2d.h"
#include "Element.h"

class Spirit : public cocos2d::Node {
protected:
    float health;          // Ѫ��
    float maxHealth;       // ���Ѫ��
    Element element;       // Ԫ������

public:
    Spirit(float health, Element element)
        : health(health), maxHealth(health), element(element) {}
    
    // ���ڲ��Ե�Ĭ�Ϲ��캯��
    Spirit() : health(100), maxHealth(100), element(Element::FIRE) {}

    virtual ~Spirit() {}

    // ��ȡ��ǰѪ��
    float getHealth() const { 
        return health; 
    }

    // ��ȡ���Ѫ��
    float getMaxHealth() const { 
        return maxHealth; 
    }

    // Ԫ������
    Element getElement() const { 
        return element; 
    }

    // �ܵ��˺�
    virtual void takeDamage(float amount) {
        health -= amount;
        if (health < 0) health = 0;
    }

    // �ָ�Ѫ��
    virtual void heal(float amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    // ��������
    virtual void attack(Spirit& target) {
        target.takeDamage(10.0f);  // �򵥵Ĺ����߼�
    }

    // ��ӡ��ǰ״̬
    virtual void printStatus() {
        CCLOG("Health: %.2f/%.2f", health, maxHealth);
        CCLOG("Element: %d", static_cast<int>(element));
    }
};

#endif // SPIRIT_H
