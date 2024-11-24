#include "Entities.h"

// ���캯��
Entities::Entities(float health, Element element)
    : health(health), maxHealth(health), element(element) {}

// Ĭ�Ϲ��캯��
Entities::Entities()
    : health(100), maxHealth(100), element(Element::FIRE) {}

// ����������
Entities::~Entities() {}

// ��ȡ��ǰѪ��
float Entities::getHealth() const {
    return health;
}

// ��ȡ���Ѫ��
float Entities::getMaxHealth() const {
    return maxHealth;
}

// ��ȡԪ������
Element Entities::getElement() const {
    return element;
}

// �ܵ��˺�
void Entities::takeDamage(float amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// �ָ�Ѫ��
void Entities::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// ����Ŀ��
void Entities::attack(Entities& target) {
    target.takeDamage(10.0f); // �򵥵Ĺ����߼�
}

// ��ӡ��ǰ״̬
void Entities::printStatus() {
    CCLOG("Health: %.2f/%.2f", health, maxHealth);
    CCLOG("Element: %s", elementToString(element).c_str());
}
