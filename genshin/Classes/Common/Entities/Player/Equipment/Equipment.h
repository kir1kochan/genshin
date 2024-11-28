#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

// װ������
class Equipment {
public:
    // ���캯��
    Equipment(const std::string& name, int power);

    // ��ȡװ������
    const std::string& getName() const;

    // ��ȡװ�������ԣ����������������
    int getPower() const;

    // ��ӡװ����Ϣ�����⺯��������������д��
    virtual void printInfo() const;

    // ��װ���������л�Ϊ JSON �ַ���
    virtual std::string saveToJson() const;

    // �� JSON �ַ�������װ������
    virtual void loadFromJson(const std::string& jsonString);

    // ������������ȷ�������������ȷ����
    virtual ~Equipment() {}

private:
    std::string name;  // װ������
    int power;         // װ���Ĺ������������
};

#endif // EQUIPMENT_H