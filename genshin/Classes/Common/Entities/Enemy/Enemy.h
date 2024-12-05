#ifndef ENEMY_H
#define ENEMY_H

#include "../Entities.h"
#include "../../Element/Element.h"
//#include "../Player/Player.h"
class Player;

class Enemy : public Entities {
private:
    int aggressionLevel;         // 敌人的攻击性等级（决定敌人的攻击频率或强度）
    float detectionRadius;       // 索敌半径
    int baseLevel;               // 基准等级
    std::string imagePath;  // 代表怪物图像文件名
    bool spriteGenerated;        // 是否已生成精灵
    bool isAlive;                // 是否存活
    int drop;                    // 掉落物id（若无则为0）
public:
    // 构造函数
    Enemy(float health, float attack, float defence, Element element, float attackRange, int aggressionLevel, float detectionRadius,
        int baseLevel, const std::string& imagePath, int drop);

    // 默认构造函数
    Enemy();

    // 获取攻击性等级
    int getAggressionLevel() const;

    // 获取索敌半径
    float getDetectionRadius() const;


    // 获取基准等级
    int getBaseLevel() const;

    // 获取精灵文件名
    std::string getSpriteFilename() const;

    // 获取存活状态
    bool getIsAlive() const;

    // 设置存活状态
    void setIsAlive(bool alive);

    // 收到伤害的函数
    void takeDamage(float amount) override;

    // 攻击敌人
    void attackTarget(Entities& target, float amount, Element element) override;

    // 敌人AI行为
    void aiBehavior(float distance, Player* player);

    // 打印敌人状态
    void printStatus() override;

    // 序列化为 JSON 字符串
    std::string saveToJson() const override;

    // 从 JSON 字符串加载数据
    void loadFromJson(const std::string& jsonString) override;

    // 带有新位置的clone
    Enemy* clone(const cocos2d::Vec2& newPosition);

    // 根据文件名生成精灵
    cocos2d::Sprite* generateSprite() ;

    // 检查是否生成了精灵
    bool isSpriteGenerated() const {
        return spriteGenerated;
    }
};

#endif // ENEMY_H
