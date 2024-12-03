#include "Potion.h"

// ���캯��
Potion::Potion(int id, const std::string& name, float effectValue)
    : Item(id, name), effectValue(effectValue) {}

// ʹ��ҩ����Ч��
// �������Բ���ʳ�����ظ�����ֻ�ǻظ�Ѫ�������� һ��ʱ������߹��١�һ��ʱ����������ѪЧ�� ��һ��ʱ���ڼӿ��ƶ��ٶȵ�
void Potion::use(Player& player) const {
   
}

// ��д��ӡ��Ϣ����
void Potion::printInfo() const {
    CCLOG("Potion Name: %s", getName().c_str());
    CCLOG("Effect Value: %.2f", effectValue);
}