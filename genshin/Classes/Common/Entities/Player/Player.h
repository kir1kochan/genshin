#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"

#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"

#include "Backpack.h"

class Player : public Entities {
private:
    int experience;  // ��Ҿ���ֵ
    int level;       // ��ҵȼ�

    // װ����ȷ��һ�����ֻ�����һ��װ����
    Weapon* weapon;           // ����
    Armor* armor;             // ����
    Accessory* accessory;     // ��Ʒ

    // ����ϵͳ
    std::vector<std::shared_ptr<Skill>> unlockedSkills; // �ѽ�������
    std::vector<std::shared_ptr<Skill>> skillBar;       // �����������3�����ܣ�

    float skillCooldownAccumulator = 0.0f;  // �ۻ�ʱ��
    const float skillCooldownInterval = 0.5f;  // ÿ�θ��µ�ʱ��������λ��

    float shieldTimeAccumulator = 0.0f;  // �ۻ�ʱ��
    const float shieldTimeInterval = 0.5f;  // ÿ�θ��µ�ʱ��������λ��
  
    float currentShield;  // ��ǰ����ֵ

   
    Backpack backpack; // ����


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
    void attack(Entities& target);

    // ����ܵ�����
    void takeDamage(float damage) override;

    // ��ӡ���״̬
    void printStatus() override;

    // ���л�Ϊ JSON �ַ���
    std::string saveToJson() const override;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;

    // װ������
    void equipWeapon(Weapon* weapon);
    void equipArmor(Armor* armor);
    void equipAccessory(Accessory* accessory);

    // ��ȡװ��
    Weapon* getWeapon() const;
    Armor* getArmor() const;
    Accessory* getAccessory() const;

    // ����ϵͳ
    void unlockSkill(const std::shared_ptr<Skill>& newSkill);   // ��������
    bool equipSkill(int skillSlot, const std::shared_ptr<Skill>& skill); // װ������
    void unequipSkill(int skillSlot);                          // ж�ؼ���
    void useSkill(int skillSlot, Entities& target);            // ʹ�ü���
    void updateSkillsCooldown(float deltaTime);                // ������ȴʱ��

    void update(float deltaTime);        // ��ʱ�������״̬��Ŀǰ���ڼ��ܺͻ��ܣ�Ҳ���Ը���������״̬

    // ���±���
    void addItemToBackpack(Item* item);
    void removeItemFromBackpack(int itemId);
    void printBackpackInfo() const;
    
};

#endif // PLAYER_H
