#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>

// װ������
class Equipment {
public:
    // ���캯��
    Equipment(int id, const std::string& name, int power);

    // ��ȡװ������
    const std::string& getName() const;

    int getId() const;

    // ��ȡװ�������ԣ����������������
    int getPower() const;

    // ��ӡװ����Ϣ�����⺯��������������д��
    virtual void printInfo() const;

    // ��װ���������л�Ϊ JSON �ַ���
    virtual std::string saveToJson() const;

    // �� JSON �ַ�������װ������
    virtual void loadFromJson(const std::string& jsonString);

    // ����Ա�������л�Ϊ JSON ��ʽ�������浽����
    virtual void Entities::saveToFile(const std::string& filePath) const;

    // �ӱ��ض�ȡ JSON �ļ�����ȡ��Ա��������
    virtual void Entities::loadFromFile(const std::string& filePath);

    // ������������ȷ�������������ȷ����
    virtual ~Equipment() {}

private:
    int id;
    std::string name;  // װ������
    int power;         // װ���Ĺ������������
};

#endif // EQUIPMENT_H