#ifndef POTION_H
#define POTION_H

#include "../Item.h"

class Player;
// ҩ����
class Potion : public Item {
public:
    // ���캯��
    Potion(int id, const std::string& name, float effectValue);

    // ʹ��ҩ����Ч��
    void use(Player& player) const;

    // ��д��ӡ��Ϣ����
    void printInfo() const override;

    // ��д saveToJson ����������ʳ����ϢΪ JSON ��ʽ
    std::string saveToJson() const override;

    // ��д loadFromJson �������� JSON ����ʳ����Ϣ
    void loadFromJson(const std::string& jsonString) override;

private:
    // ҩ��Ч��ֵ�����磺�ָ�Ѫ�������ӹ������ȣ�
    // ҩЧ�����Ϳ���ͨ��id����ҩƷ������ȷ��
    float effectValue;     
    
    
};

#endif // POTION_H