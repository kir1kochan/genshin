#ifndef ENTITIES_H
#define ENTITIES_H

#include "cocos2d.h"
#include "../Element/Element.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <map>

class Entities : public cocos2d::Node {
protected:
    float health;          // 当前血量
    float maxHealth;       // 最大血量
    float attack;
    float defence;
    Element element;       // 元素属性

    float attackRange;     // 攻击范围

    float baseAttack;


    // 存储实体的状态，如冰冻、中毒、燃烧等
    std::map<std::string, float> statusEffects = {
          {"Frozen", 0.0f},    // 冰冻状态，初始持续时间为0
          {"Burning", 0.0f},   // 燃烧状态
          {"Poisoned", 0.0f},  // 中毒状态
          {"Paralyzed", 0.0f}, // 麻痹状态
          {"Weakened", 0.0f},  // 削弱状态
          {"Drenched", 0.0f},  // 湿润状态
    };

    // 攻击冷却
    float attackCooldownAccumulator = 0.0f;   // 累积时间
    const float attackCooldownInterval=0.5f; // 每次更新的时间间隔，单位秒
    float currentCooldown = 0.0f;  // 当前冷却时间

public:
    // 构造函数
    Entities(float health, float attack, float defence, Element element, float attackRange);

    // 默认构造函数
    Entities();

    // 虚析构函数
    virtual ~Entities();

    // 获取当前血量
    float getHealth() const;

    // 获取最大血量
    float getMaxHealth() const;

    // 获取元素类型
    Element getElement() const;

    // 受到伤害
    virtual void takeDamage(float amount);

    // 恢复血量
    virtual void heal(float amount);

    // 攻击目标
    virtual void attackTarget(Entities& target, float amount, Element element);


    // 打印当前状态
    virtual void printStatus();

    // 更新攻击冷却
    void updateAttackCooldown(float deltaTime);

    // 判断是否可以攻击
    bool canAttack() const;

    // 获取攻击范围
    virtual float getAttackRange() const;

    // 判断目标是否在攻击范围内
    bool attackInRange(Entities& target);

    // 每帧更新逻辑
    virtual void update(float deltaTime);

    // 将所有成员变量序列化为 JSON 格式，生成 JSON 字符串
    virtual std::string saveToJson() const;

    // 从 JSON 字符串中读取成员变量序列
    virtual void loadFromJson(const std::string& jsonString);

    // 将成员变量序列化为 JSON 格式，并保存到本地
    virtual void saveToFile(const std::string& filePath) const;

    // 从本地读取 JSON 文件，读取成员变量序列
    virtual void loadFromFile(const std::string& filePath);

    // 添加状态效果
    void applyStatusEffect(const std::string& effect, float duration);

    // 移除状态效果
    void removeStatusEffect(const std::string& effect);

    // 更新状态效果
    virtual void updateStatusEffects(float deltaTime);
    bool hasStatusEffect(const std::string& effect) const;
    void applyElementalEffects(Entities& target, Element skillElement);
};

#endif // ENTITIES_H
