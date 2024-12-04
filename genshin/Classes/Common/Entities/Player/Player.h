#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"
#include"../Enemy/Enemy.h"
#include "Classes/Common/Item/Equipment/Weapon/Weapon.h"
#include "Classes/Common/Item/Equipment/Armor/Armor.h"
#include "Classes/Common/Item/Equipment/Accessory/Accessory.h"
#include "Skill/AttackSkill/AttackSkill.h"
#include "Skill/HealSkill/HealSkill.h"
#include "Skill/ShieldSkill/ShieldSkill.h"
#include "Classes/Common/Backpack/Backpack.h"



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
    float shieldTime;     //���׵ĳ���ʱ��
    float CDtoSet = 1.0f;       // ����cd
   
    Backpack backpack; // ����


public:
    Player(float health, Element element, float attackRange);
    Player();  // Ĭ�Ϲ��캯��

    // ����
    void levelUp();

    // ��ȡ��ǰ�ȼ�
    int getLevel() const;

    // ��������
    void gainExperience(int exp);

    //�ı�Ԫ��
    void chanegElement(Element newElement);

    // ������ҵ���ʱ����Ԫ�����
    void attackTarget(Enemy& target);

    // ����ܵ�����
    void takeDamage(float damage) override;

    void heal(float healAmount);

    void setShield(float shield, float Time);

    // ��ӡ���״̬
    void printStatus() override;

    // ���л�Ϊ JSON �ַ���
    std::string saveToJson() const override;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;

    // װ������
    void equipWeapon(Weapon* newWeapon);
    void equipArmor(Armor* newArmor);
    void equipAccessory(Accessory* newAccessory);


    // ��ȡ����������Χ
    float getWeaponAttackRange() const;
    // ��ȡ��������Ƶ��


    float getWeaponAttackSpeed() const;
    // ����ϵͳ
    void unlockSkill(const std::shared_ptr<Skill>& newSkill);   // ��������
    bool equipSkill(int skillSlot, const std::shared_ptr<Skill>& skill); // װ������
    void unequipSkill(int skillSlot);                          // ж�ؼ���
    void useSkill(int skillSlot, Enemy& target);            // ʹ�ü���
    void updateSkillsCooldown(float deltaTime);                // ������ȴʱ��

    float getShield() const;

    void updateshieldTime(float deltaTime);

    void update(float deltaTime);        // ��ʱ�������״̬��Ŀǰ���ڼ��ܺͻ��ܣ�Ҳ���Ը���������״̬

    // ���±���
    void addItemToBackpack(int id, int count);
    void removeItemFromBackpack(int itemId);
    void printBackpackInfo() const;
    
};

#endif // PLAYER_H
