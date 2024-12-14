#ifndef POTION_H
#define POTION_H

#include "../Item.h"

class Player;
// 药剂类,当前设计为恢复人物的体力值
class Potion : public Item {
public:
    // 构造函数
    Potion(int id, const std::string& name, float healAmount);

    // 使用药剂
    void use(Player& user) const;

    // 获取恢复量
    float getHealAmount() const;

    // 重写打印信息函数
    void printInfo() const override;

    // 重写 saveToJson 函数，保存食物信息为 JSON 格式
    std::string saveToJson() const override;

    // 重写 loadFromJson 函数，从 JSON 加载食物信息
    void loadFromJson(const std::string& jsonString) override;

private:
    float healAmount; // 恢复的量 
};

#endif // POTION_H