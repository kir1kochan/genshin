#ifndef PLAYER_H
#define PLAYER_H

#include "../Entities.h"
#include"../Enemy/Enemy.h"
#include "Classes/Common/Item/Equipment/Weapon/Weapon.h"
#include "Classes/Common/Item/Equipment/Armor/Armor.h"
#include "Classes/Common/Item/Equipment/Accessory/Accessory.h"
#include "Classes/UI/Hud.h"
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
    
    Sprite* shieldSprite = nullptr;  // ������ʾ���ܵľ���
    float currentShield;  // ��ǰ����ֵ
    float shieldTime;     //���׵ĳ���ʱ��
    float CDtoSet = 1.0f;       // ����cd
   
    Backpack* backpack; // ����

    // �洢���ܵ����ж���
    std::vector<Vector<SpriteFrame*>> skillAnimations;

    Hud* hud=nullptr;
public:
    Player( Element element, float attackRange);
    Player();  // Ĭ�Ϲ��캯��
    Player(cocos2d::Sprite* sprite);
    Player(float health, cocos2d::Sprite* sprite); // ����������Ҫ�õ���player���캯��

    // ����
    void levelUp();

    // ��ȡ��ǰ�ȼ�
    int getLevel() const;
    // ��ȡ����ֵ
    float getExp() const { return experience; }
    // ��ȡ������
    float getAttack() const { return attack; }
    // ��ȡ������
    float getDefence() const { return defence; }
    // ��ȡװ������
    std::shared_ptr<Weapon> getWeapon() const { return weapon; }
    std::shared_ptr<Armor> getArmor() const { return armor; }
    std::shared_ptr<Accessory> getAccessory() const { return accessory; }

    // ��ȡ��������
    std::vector<std::shared_ptr<Skill>>& getunlockedSkills() { return unlockedSkills; }
    std::vector<std::shared_ptr<Skill>>& getskillBar() { return skillBar; }

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

    void removeShield();

    void update(float deltaTime);        // ��ʱ�������״̬��Ŀǰ���ڼ��ܺͻ��ܣ�Ҳ���Ը���������״̬

    // ���±���
    void addItemToBackpack(int id, int count);
    void removeItemFromBackpack(int itemId);
    void printBackpackInfo() const;

    Backpack* getBackpack() { return backpack; }

    std::shared_ptr<Skill> creatSkillById(int id, const std::string& jsonString);

    // ����ֵ��ط���
    void regenerateStamina(float amount);  // �ָ�����
    void reduceStamina(float amount);      // ��������
    float getStamina() const;              // ��ȡ��ǰ����
    float getMaxStamina() const { return maxStamina; }              // ��ȡ�������
    void updateStamina(float deltaTime);   // ÿ֡��������
    
    // ���ؼ��ܶ���
    void loadSkillAnimations();

    void testSkill();

    void lauchSkill(int skillSlot, Enemy& target);

    Hud* getHud() {
        return hud;
    }
};

#endif // PLAYER_H
