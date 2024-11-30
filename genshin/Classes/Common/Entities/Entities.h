#ifndef ENTITIES_H
#define ENTITIES_H

#include "cocos2d.h"
#include "../Element/Element.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include<Map.h>

class Entities : public cocos2d::Node {
protected:
    float health;          // 当前血量
    float maxHealth;       // 最大血量
    float attack;
    float defence;
    Element element;       // 元素属性

    // 存储实体的状态，如冰冻、中毒、燃烧等
    std::map<std::string, float> statusEffects; // 状态名 -> 持续时间

public:
    // 构造函数
    Entities(float health, float attack, float defence, Element element);

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
    virtual void attack(Entities& target, float amount, Element element);

    // 打印当前状态
    virtual void printStatus();

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
};

#endif // ENTITIES_H
