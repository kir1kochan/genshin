#ifndef POTION_H
#define POTION_H

#include "../Item.h"
#include "Classes/Common/Entities/Player/Player.h"

// ҩ����
class Potion : public Item {
public:
    // ���캯��
    Potion(int id, const std::string& name, float effectValue, const std::string& effectType);

    // ʹ��ҩ����Ч��
    void use(Player& player) const;

    // ��д��ӡ��Ϣ����
    void printInfo() const override;

private:
    // ҩ��Ч��ֵ�����磺�ָ�Ѫ�������ӹ������ȣ�
    // ҩЧ�����Ϳ���ͨ��id����ҩƷ������ȷ��
    float effectValue;     
    
    
};

#endif // POTION_H