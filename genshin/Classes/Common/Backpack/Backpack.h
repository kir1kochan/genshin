#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>
#include "Item.h"

class Backpack {
public:
    // �����Ʒ
    void addItem(Item* item);

    // �Ƴ���Ʒ
    void removeItem(int itemId);

    // ��ӡ������Ϣ
    void printInfo() const;

    // �������������л�Ϊ JSON �ַ���
    std::string saveToJson() const;

    // �� JSON �ַ������ر�������
    void loadFromJson(const std::string& jsonString);

    // ���������浽�ļ�
    void saveToFile(const std::string& filePath) const;

    // ���ļ����ر�������
    void loadFromFile(const std::string& filePath);

private:
    std::vector<Item*> items; // �����е���Ʒ����
};

#endif // BACKPACK_H
