#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "Equipment.h"

// ��Ʒ�࣬�̳���װ����
class Accessory : public Equipment {
public:
    // ���캯��
    Accessory(int id, const std::string& name, int power);

    // ��д��ӡ��Ϣ����
    void printInfo() const override;
};

#endif // ACCESSORY_H