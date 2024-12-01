#ifndef ENEMY_H
#define ENEMY_H

#include "../Entities.h"

class Enemy : public Entities {
private:
    int aggressionLevel;  // 敌人的攻击性等级（决定敌人的攻击频率或强度）
    float detectionRadius; // 索敌半径
    int baseLevel;        // 基准等级
    std::string spriteFilename;  // 代表怪物图像文件名
    bool spriteGenerated; // 是否已生成精灵

public:
    Enemy(float health, Element element, int aggressionLevel, float detectionRadius, int baseLevel, const std::string& spriteFilename);
    Enemy();  // 默认构造函数

    int getAggressionLevel() const;
    float getDetectionRadius() const;
    int getBaseLevel() const;
    std::string getSpriteFilename() const;

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

    // 带有新位置的clone
    Enemy* clone(const cocos2d::Vec2& newPosition);

    // 根据文件名生成精灵
    cocos2d::Sprite* generateSprite() const;

    // 检查是否生成了精灵
    bool isSpriteGenerated() const {
        return spriteGenerated;
    }
};

#endif // ENEMY_H