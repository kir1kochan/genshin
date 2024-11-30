#include "Accessory.h"

// ���캯��
Accessory::Accessory(int id, const std::string& name, int power)
    : Equipment(id, name, power) {}

// ��д��ӡ��Ϣ����
void Accessory::printInfo() const {
    CCLOG("Accessory Name: %s", getName().c_str());
    CCLOG("Power: %d", getPower());
}