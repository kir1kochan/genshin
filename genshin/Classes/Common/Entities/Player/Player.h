#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"
#include"../Enemy/Enemy.h"
#include "Classes/Common/Item/Equipment/Weapon/Weapon.h"
#include "Classes/Common/Item/Equipment/Armor/Armor.h"
#include "Classes/Common/Item/Equipment/Accessory/Accessory.h"
//#include "Skill/Skill.h"
//#include "Skill/AttackSkill/AttackSkill.h"
//#include "Skill/HealSkill/HealSkill.h"
//#include "Skill/ShieldSkill/ShieldSkill.h"
#include "Classes/Common/Backpack/Backpack.h"
class AttackSkill;
class HealSkill;
class ShiedSkill;
class Skill;

class Player : public Entities {
private:
    int experience;  // ��Ҿ���ֵ
    int level;       // ��ҵȼ�

    float stamina;      // ��ǰ����
    float maxStamina;   // �������

    // װ����ȷ��һ�����ֻ�����һ��װ����
    std::shared_ptr<Weapon> weapon;      //����
    std::shared_ptr<Armor> armor;         //����
    std::shared_ptr<Accessory> accessory;  //��Ʒ
  
    // ����ϵͳ
    std::vector<std::shared_ptr<Skill>> unlockedSkills; // �ѽ�������
    std::vector<std::shared_ptr<Skill>> skillBar;       // �����������4�����ܣ�
   

    float skillCooldownAccumulator = 0.0f;  // �ۻ�ʱ��
    const float skillCooldownInterval = 0.5f;  // ÿ�θ��µ�ʱ��������λ��

    float shieldTimeAccumulator = 0.0f;  // �ۻ�ʱ��
    const float shieldTimeInterval = 0.5f;  // ÿ�θ��µ�ʱ��������λ��
  
    float currentShield;  // ��ǰ����ֵ
    float shieldTime;     //���׵ĳ���ʱ��
    float CDtoSet = 1.0f;       // ����cd
   
    Backpack backpack; // ����


public:
    Player( Element element, float attackRange);
    Player();  // Ĭ�Ϲ��캯��
    Player(cocos2d::Sprite* sprite);

    // ����
    void levelUp();

    // ��ȡ��ǰ�ȼ�
    int getLevel() const;

    // ��������
    void gainExperience(int exp);

    // ��ȡ�������辭��
    float getExperienceForNextLevel() const;

    //�ı�Ԫ��
    void chanegElement(Element newElement);

    // ������ҵ���ʱ����Ԫ�����
    void attackTarget(Enemy& target);

    void attackTargetBySkill(Enemy& target, float attackValue, Element skillElment);

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
    void equipWeapon(std::shared_ptr<Weapon> newWeapon);
    void equipArmor(std::shared_ptr<Armor> newArmor);
    void equipAccessory(std::shared_ptr<Accessory> newAccessory);
    // ����װ��
    void unequipWeapon();
    void unequipArmor();
    void unequipAccessory();

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
    void checkAndUnlockSkills();                               // ���ݵ�ǰ�ȼ���������

    float getShield() const;

    void updateshieldTime(float deltaTime);

    void update(float deltaTime);        // ��ʱ�������״̬��Ŀǰ���ڼ��ܺͻ��ܣ�Ҳ���Ը���������״̬

    // ���±���
    void addItemToBackpack(int id, int count);
    void removeItemFromBackpack(int itemId);
    void printBackpackInfo() const;

    std::shared_ptr<Skill> creatSkillById(int id, const std::string& jsonString);

    // ����ֵ��ط���
    void regenerateStamina(float amount);  // �ָ�����
    void reduceStamina(float amount);      // ��������
    float getStamina() const;              // ��ȡ��ǰ����
    void updateStamina(float deltaTime);   // ÿ֡��������
    
};

#endif // PLAYER_H
