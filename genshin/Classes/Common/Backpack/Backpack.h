#ifndef BACKPACK_H
#define BACKPACK_H

#include <unordered_map>
#include <string>
#include "Item.h"
#include "Player.h"

class Backpack {
public:

    // ���캯��
    Backpack(const std::vector<Item*>& itemPointers);
   
    // ͨ����Ʒָ�������Ʒ
    void addItem(Item* item, int count = 1);

    // ͨ����ƷID�����Ʒ
    void addItemById(int itemId, int count = 1);

    // ͨ����Ʒָ���Ƴ���Ʒ
    void removeItem(Item* item, int count = 1);

    // ͨ����ƷID�Ƴ���Ʒ
    void removeItemById(int itemId, int count = 1);

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

    friend:class Player;
private:
    std::unordered_map<Item*, int> items;  // ��Ʒָ���������ӳ��
    std::unordered_map<int, Item*> idToItemMap;  // ��ƷID����Ʒָ���ӳ��
};

#endif // BACKPACK_H
