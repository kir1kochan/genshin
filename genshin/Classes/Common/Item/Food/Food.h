#ifndef FOOD_H
#define FOOD_H

#include "Item.h"
#include"Player.h"

class Food : public Item {
public:
    // ���캯��
    Food(int id, const std::string& name, float healAmount);

    // ��ȡ�ָ���
    float getHealAmount() const;

    // ��д printInfo ��������ӡʳ����Ϣ
    void printInfo() const override;

    // ��д saveToJson ����������ʳ����ϢΪ JSON ��ʽ
    std::string saveToJson() const override;

    // ��д loadFromJson �������� JSON ����ʳ����Ϣ
    void loadFromJson(const std::string& jsonString) override;

    void use(Player* user);

private:
    float healAmount; // �ָ�����
};

#endif // FOOD_H