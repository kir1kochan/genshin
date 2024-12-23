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
    int experience;  // 玩家经验值
    int level;       // 玩家等级

    float stamina;      // 当前体力
    float maxStamina;   // 最大体力

    // 装备（确保一个玩家只能佩戴一个装备）
    std::shared_ptr<Weapon> weapon;      //武器
    std::shared_ptr<Armor> armor;         //护具
    std::shared_ptr<Accessory> accessory;  //饰品
  
    // 技能系统
    std::vector<std::shared_ptr<Skill>> unlockedSkills; // 已解锁技能
    std::vector<std::shared_ptr<Skill>> skillBar;       // 技能栏（最多4个技能）
   

    float skillCooldownAccumulator = 0.0f;  // 累积时间
    const float skillCooldownInterval = 0.5f;  // 每次更新的时间间隔，单位秒

    float shieldTimeAccumulator = 0.0f;  // 累积时间
    const float shieldTimeInterval = 0.5f;  // 每次更新的时间间隔，单位秒
    
    Sprite* shieldSprite = nullptr;  // 用于显示护盾的精灵
    float currentShield;  // 当前护甲值
    float shieldTime;     //护甲的持续时间
    float CDtoSet = 1.0f;       // 攻击cd
   
    Backpack* backpack; // 背包

    // 存储技能的所有动画
    std::vector<Vector<SpriteFrame*>> skillAnimations;

    Hud* hud=nullptr;
public:
    Player( Element element, float attackRange);
    Player();  // 默认构造函数
    Player(cocos2d::Sprite* sprite);
    Player(float health, cocos2d::Sprite* sprite); // 护送任务需要用到的player构造函数

    // 升级
    void levelUp();

    // 获取当前等级
    int getLevel() const;
    // 获取经验值
    float getExp() const { return experience; }
    // 获取攻击力
    float getAttack() const { return attack; }
    // 获取防御力
    float getDefence() const { return defence; }
    // 获取装备内容
    std::shared_ptr<Weapon> getWeapon() const { return weapon; }
    std::shared_ptr<Armor> getArmor() const { return armor; }
    std::shared_ptr<Accessory> getAccessory() const { return accessory; }

    // 获取技能内容
    std::vector<std::shared_ptr<Skill>>& getunlockedSkills() { return unlockedSkills; }
    std::vector<std::shared_ptr<Skill>>& getskillBar() { return skillBar; }

    // 经验增加
    void gainExperience(int exp);

    // 获取升级所需经验
    float getExperienceForNextLevel() const;

    //改变元素
    void chanegElement(Element newElement);

    // 攻击玩家敌人时根据元素相克
    void attackTarget(Enemy& target);

    void attackTargetBySkill(Enemy& target, float attackValue, Element skillElment);

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
    void equipWeapon(std::shared_ptr<Weapon> newWeapon);
    void equipArmor(std::shared_ptr<Armor> newArmor);
    void equipAccessory(std::shared_ptr<Accessory> newAccessory);
    // 脱下装备
    void unequipWeapon();
    void unequipArmor();
    void unequipAccessory();

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
    void checkAndUnlockSkills();                               // 根据当前等级解锁技能

    float getShield() const;

    void updateshieldTime(float deltaTime);

    void removeShield();

    void update(float deltaTime);        // 定时更新玩家状态，目前用于技能和护盾，也可以更新其他的状态

    // 更新背包
    void addItemToBackpack(int id, int count);
    void removeItemFromBackpack(int itemId);
    void printBackpackInfo() const;

    Backpack* getBackpack() { return backpack; }

    std::shared_ptr<Skill> creatSkillById(int id, const std::string& jsonString);

    // 体力值相关方法
    void regenerateStamina(float amount);  // 恢复体力
    void reduceStamina(float amount);      // 消耗体力
    float getStamina() const;              // 获取当前体力
    float getMaxStamina() const { return maxStamina; }              // 获取最大体力
    void updateStamina(float deltaTime);   // 每帧更新体力
    
    // 加载技能动画
    void loadSkillAnimations();

    void testSkill();

    void lauchSkill(int skillSlot, Enemy& target);

    Hud* getHud() {
        return hud;
    }
};

#endif // PLAYER_H
