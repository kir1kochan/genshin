#include "Armor.h"

// ���캯��
Armor::Armor(const std::string& name, int defense)
    : Equipment(name, defense) {}

// ��д��ӡ��Ϣ����
void Armor::printInfo() const {
    CCLOG("Armor Name: %s", getName().c_str());
    CCLOG("Defense: %d", getPower());
}