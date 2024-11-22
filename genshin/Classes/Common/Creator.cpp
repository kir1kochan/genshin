#include "Creator.h"

// ���캯��
Creator::Creator(float health, Element element)
    : health(health), maxHealth(health), element(element) {}

// Ĭ�Ϲ��캯��
Creator::Creator()
    : health(100), maxHealth(100), element(Element::FIRE) {}

// ����������
Creator::~Creator() {}

// ��ȡ��ǰѪ��
float Creator::getHealth() const {
    return health;
}

// ��ȡ���Ѫ��
float Creator::getMaxHealth() const {
    return maxHealth;
}

// ��ȡԪ������
Element Creator::getElement() const {
    return element;
}

// �ܵ��˺�
void Creator::takeDamage(float amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// �ָ�Ѫ��
void Creator::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// ����Ŀ��
void Creator::attack(Creator& target) {
    target.takeDamage(10.0f); // �򵥵Ĺ����߼�
}

// ��ӡ��ǰ״̬
void Creator::printStatus() {
    CCLOG("Health: %.2f/%.2f", health, maxHealth);
    CCLOG("Element: %s", elementToString(element).c_str());
}
