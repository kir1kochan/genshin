#ifndef WEAPON_H
#define WEAPON_H

#include "Equipment.h"

// �����࣬�̳���װ����

//���Խ�һ�����ƣ����չ������뻮��
class Weapon : public Equipment {
public:
    // ���캯��
    Weapon(int id, const std::string& name, int attackPower);

    // ��д��ӡ��Ϣ����
    void printInfo() const override;
};

#endif // WEAPON_H