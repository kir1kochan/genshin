#include "Armor.h"

// ���캯��
Armor::Armor(int id, const std::string& name, int power)
    : Equipment(id, name, power) {}

// ��д��ӡ��Ϣ����
void Armor::printInfo() const {
    CCLOG("Armor Name: %s", getName().c_str());
    CCLOG("Defense: %d", getPower());
}