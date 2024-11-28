#ifndef ENEMY_H
#define ENEMY_H

#include "../Entities.h"

class Enemy : public Entities {
private:
    int aggressionLevel;  // 敌人的攻击性等级（决定敌人的攻击频率或强度）
    float detectionRadius; // 索敌半径
    int baseLevel;        // 基准等级
    int monsterID;        // 怪物编号

public:
    Enemy(float health, Element element, int aggressionLevel, float detectionRadius, int baseLevel, int monsterID);
    Enemy();  // 默认构造函数

    int getAggressionLevel() const;
    float getDetectionRadius() const;
    int getBaseLevel() const;
    int getMonsterID() const;

    // 攻击敌人
    void attack(Entities& target) override;

    // 敌人AI行为
    void aiBehavior();

    // 打印敌人状态
    void printStatus() override;

    // 序列化为 JSON 字符串
    std::string saveToJson() const override;

    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString) override;
};

#endif // ENEMY_H
