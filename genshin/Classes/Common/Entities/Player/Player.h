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
    int experience;  // 玩家经验值
    int level;       // 玩家等级

    // 装备（确保一个玩家只能佩戴一个装备）
    Weapon* weapon;           // 武器
    Armor* armor;             // 护甲
    Accessory* accessory;     // 饰品
  
    // 技能系统
    std::vector<std::shared_ptr<Skill>> unlockedSkills; // 已解锁技能
    std::vector<std::shared_ptr<Skill>> skillBar;       // 技能栏（最多3个技能）

    float skillCooldownAccumulator = 0.0f;  // 累积时间
    const float skillCooldownInterval = 0.5f;  // 每次更新的时间间隔，单位秒

    float shieldTimeAccumulator = 0.0f;  // 累积时间
    const float shieldTimeInterval = 0.5f;  // 每次更新的时间间隔，单位秒
  
    float currentShield;  // 当前护甲值
    float shieldTime;     //护甲的持续时间
    float CDtoSet = 1.0f;       // 攻击cd
   
    Backpack backpack; // 背包


public:
    Player(float health, Element element, float attackRange);
    Player();  // 默认构造函数

    // 升级
    void levelUp();

    // 获取当前等级
    int getLevel() const;

    // 经验增加
    void gainExperience(int exp);

    //改变元素
    void chanegElement(Element newElement);

    // 攻击玩家敌人时根据元素相克
    void attackTarget(Enemy& target);

    // 玩家受到攻击
    void takeDamage(float damage) override;

    void heal(float healAmount);

    void setShield(float shield, float Time);

    // 打印玩家状态
    void printStatus() override;

    // 序列化为 JSON 字符串
    std::string saveToJson() const override;

    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString) override;

    // 装备管理
    void equipWeapon(Weapon* newWeapon);
    void equipArmor(Armor* newArmor);
    void equipAccessory(Accessory* newAccessory);


    // 获取武器攻击范围
    float getWeaponAttackRange() const;
    // 获取武器攻击频率


    float getWeaponAttackSpeed() const;
    // 技能系统
    void unlockSkill(const std::shared_ptr<Skill>& newSkill);   // 解锁技能
    bool equipSkill(int skillSlot, const std::shared_ptr<Skill>& skill); // 装备技能
    void unequipSkill(int skillSlot);                          // 卸载技能
    void useSkill(int skillSlot, Enemy& target);            // 使用技能
    void updateSkillsCooldown(float deltaTime);                // 更新冷却时间

    float getShield() const;

    void updateshieldTime(float deltaTime);

    void update(float deltaTime);        // 定时更新玩家状态，目前用于技能和护盾，也可以更新其他的状态

    // 更新背包
    void addItemToBackpack(int id, int count);
    void removeItemFromBackpack(int itemId);
    void printBackpackInfo() const;
    
};

#endif // PLAYER_H
