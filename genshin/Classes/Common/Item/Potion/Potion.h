#ifndef POTION_H
#define POTION_H

#include "../Item.h"

class Player;
// ҩ����,��ǰ���Ϊ�ָ����������ֵ
class Potion : public Item {
public:
    // ���캯��
    Potion(int id, const std::string& name, float healAmount);

    // ʹ��ҩ��
    void use(Player& user) const;

    // ��ȡ�ָ���
    float getHealAmount() const;

    // ��д��ӡ��Ϣ����
    void printInfo() const override;

    // ��д saveToJson ����������ʳ����ϢΪ JSON ��ʽ
    std::string saveToJson() const override;

    // ��д loadFromJson �������� JSON ����ʳ����Ϣ
    void loadFromJson(const std::string& jsonString) override;

private:
    float healAmount; // �ָ����� 
};

#endif // POTION_H