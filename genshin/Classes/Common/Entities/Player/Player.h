#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"

#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"

class Player : public Entities {
private:
    int experience;  // ��Ҿ���ֵ
    int level;       // ��ҵȼ�

    // װ����ȷ��һ�����ֻ�����һ��װ����
    Weapon* weapon;           // ����
    Armor* armor;             // ����
    Accessory* accessory;     // ��Ʒ

public:
    Player(float health, Element element);
    Player();  // Ĭ�Ϲ��캯��

    // ����
    void levelUp();

    // ��ȡ��ǰ�ȼ�
    int getLevel() const;

    // ��������
    void gainExperience(int exp);

    // ������ҵ���ʱ����Ԫ�����
    void attack(Entities& target) override;

    // ��Ҷ��صļ���
    void castSkill();

    // ��ӡ���״̬
    void printStatus() override;

    // װ������
    void equipWeapon(Weapon* weapon);
    void equipArmor(Armor* armor);
    void equipAccessory(Accessory* accessory);

    // ��ȡװ��
    Weapon* getWeapon() const;
    Armor* getArmor() const;
    Accessory* getAccessory() const;
};

#endif // PLAYER_H
