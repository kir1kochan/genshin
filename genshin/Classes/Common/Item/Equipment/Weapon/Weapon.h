#ifndef WEAPON_H
#define WEAPON_H

#include "../Equipment.h"
class  Player;

// �����࣬�̳���װ����
class Weapon : public Equipment {
private:
    float attackRange;  // ����������Χ
    float attackSpeed;  // ��������Ƶ�ʣ�ÿ�빥��������

public:
    // ���캯��
    Weapon(int id, const std::string& name, int attackPower, float attackRange, float attackSpeed);

    // ��ȡ������Χ
    float getAttackRange() const;

    // ��ȡ����Ƶ��
    float getAttackSpeed() const;

    // ��д��ӡ��Ϣ����
    void printInfo() const override;

    // �������������л�Ϊ JSON �ַ���
    std::string saveToJson() const override;

    // �� JSON �ַ���������������
    void loadFromJson(const std::string& jsonString) override;
};

#endif // WEAPON_H