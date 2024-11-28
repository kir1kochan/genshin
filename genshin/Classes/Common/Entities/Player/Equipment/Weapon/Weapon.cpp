#include "Weapon.h"

// ���캯��
Weapon::Weapon(const std::string& name, int attackPower)
    : Equipment(name, attackPower) {}

// ��д��ӡ��Ϣ����
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
}