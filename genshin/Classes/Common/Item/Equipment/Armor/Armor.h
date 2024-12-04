#ifndef ARMOR_H
#define ARMOR_H

#include "../Equipment.h"
#include "Classes/Common/Entities/Player/Player.h"

// �����࣬�̳���װ����
class Armor : public Equipment {
public:
    // ���캯��
    Armor(int id, const std::string& name, int defense);

    // ��д��ӡ��Ϣ����
    void printInfo() const override;
};

#endif // ARMOR_H
