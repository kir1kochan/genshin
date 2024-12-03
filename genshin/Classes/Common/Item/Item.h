#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <fstream>
#include <sstream>
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Classes/Common/Entities/Player/Player.h"
#include "Potion/Potion.h"
#include "Food/Food.h"

//��Ʒ���Լ̳�Node�࣬���Ը�����Ҫ�ٸĽ�
class Item {
public:
    // ���캯��
    Item(int id, const std::string& name);

    // ��ȡ��Ʒ������
    const std::string& getName() const;

    // ��ȡ��Ʒ�� ID
    int getId() const;

    // ��ӡ��Ʒ��Ϣ�����⺯��������������д��
    virtual void printInfo() const;

    // ����Ʒ�������л�Ϊ JSON �ַ���
    virtual std::string saveToJson() const;

    // �� JSON �ַ���������Ʒ����
    virtual void loadFromJson(const std::string& jsonString);

    // ���ļ�������Ʒ����
    virtual void loadFromFile(const std::string& filePath);

    // ����Ʒ���ݱ��浽�ļ�
    virtual void saveToFile(const std::string& filePath) const;

    // ���� ID
    // �����Ľ���������Ը�����Ϸ����Ƹ����걸�ı�����ϵ�������Ժ궨��ķ�ʽ����
   // ���� ID �����ض�Ӧ����Ʒ����
    Item* createItemById(int id, const std::string& jsonString);

protected:
    int id;        // ��Ʒ ID
    std::string name;  // ��Ʒ����
};

#endif // ITEM_H