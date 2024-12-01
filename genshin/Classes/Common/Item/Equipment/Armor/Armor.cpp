#include "Armor.h"

// ���캯��
Armor::Armor(int id, const std::string& name, int defense)
    : Equipment(id, name, defense) {}

// ��д��ӡ��Ϣ����
void Armor::printInfo() const {
    CCLOG("Armor Name: %s", getName().c_str());
    CCLOG("Defense: %d", getPower());
}