#ifndef BACKPACK_H
#define BACKPACK_H

#include <unordered_map>
#include <string>
#include "../Item/Item.h"
//#include "../Entities/Player/Player.h"
#include<vector>
#include "Classes/Common/Item/Equipment/Weapon/Weapon.h"
#include "Classes/Common/Item/Equipment/Armor/Armor.h"
#include "Classes/Common/Item/Equipment/Accessory/Accessory.h"
#include "Classes/Common/Item/Food/Food.h"
#include "Classes/Common/Item/Potion/Potion.h"
#include <memory>
// ǰ��������
class Item;
class Potion;
class Food;
class Weapon;
class Armor;
class Accessory;
class Player;

class Backpack {
public:
    // Ĭ�Ϲ��캯��
    Backpack();

    // ���캯��
    Backpack(const std::vector<std::shared_ptr<Item>>& itemPointers);

    // ͨ����Ʒָ�������Ʒ
    void addItem(const std::shared_ptr<Item>& item, int count = 1);

    // ͨ����ƷID�����Ʒ
    void addItem(int itemId, int count = 1);

    // ͨ����Ʒָ���Ƴ���Ʒ
    void removeItem(const std::shared_ptr<Item>& item, int count = 1);

    // ͨ����ƷID�Ƴ���Ʒ
    void removeItem(int itemId, int count = 1);

    // ����id���ʵ��
    std::shared_ptr<Item> findItemById(int id);
    
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

    // ���� ID
    // �����Ľ���������Ը�����Ϸ����Ƹ����걸�ı�����ϵ�������Ժ궨��ķ�ʽ����
    // ���� ID �����ض�Ӧ����Ʒ����
    std::shared_ptr<Item> createItemById(int id, const std::string& jsonString);

    // ������ƷID��ȡ��Ʒ����
    int getItemCountById(int itemId) const;

    // ���͹㲥����ʶ����ʳ������б䶯
    void sendFoodBroadcast();

    // ��ȡ�������
    int getCoins() const;

    // ���ӽ��
    void addCoins(int amount);

    // ʹ�ý�ң��ɹ�����true��ʧ�ܷ���false����Ҳ��㣩
    bool useCoins(int amount);

    const std::unordered_map<std::shared_ptr<Item>, int>& getItems() const;

    friend class Player;
private:
    std::unordered_map<std::shared_ptr<Item>, int> items;  // ��Ʒָ���������ӳ��
    std::unordered_map<int, std::shared_ptr<Item>> idToItemMap;  // ��ƷID����Ʒָ���ӳ��
    int coins;  // �洢�������
};

#endif // BACKPACK_H
