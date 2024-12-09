#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "../Equipment.h"
#include "Classes/Common/Element/Element.h"
class  Player;


// ��Ʒ�࣬�̳���װ����
class Accessory : public Equipment {
public:
    // ���캯��
    Accessory(int id, const std::string& name, int power,Element element);

    // ��д��ӡ��Ϣ����
    void printInfo() const override;

    Element getElement();

    // ����Ϊ JSON �ַ���
    std::string saveToJson() const;

    // �� JSON �ַ�������
    void loadFromJson(const std::string& jsonString);
protected:
    Element element;  // ��Ʒ��Ԫ������
};

#endif // ACCESSORY_H  