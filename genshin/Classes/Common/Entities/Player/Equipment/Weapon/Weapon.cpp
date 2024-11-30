#include "Weapon.h"

// ���캯��
Weapon::Weapon(int id, const std::string& name, int attackPower)
    : Equipment(id, name, attackPower) {}

// ��д��ӡ��Ϣ����
void Weapon::printInfo() const {
    CCLOG("Weapon Name: %s", getName().c_str());
    CCLOG("Attack Power: %d", getPower());
}