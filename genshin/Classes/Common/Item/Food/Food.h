#ifndef FOOD_H
#define FOOD_H

#include "Item.h"
#include"Player.h"

class Food : public Item {
public:
    // 构造函数
    Food(int id, const std::string& name, float healAmount);

    // 获取恢复量
    float getHealAmount() const;

    // 重写 printInfo 函数，打印食物信息
    void printInfo() const override;

    // 重写 saveToJson 函数，保存食物信息为 JSON 格式
    std::string saveToJson() const override;

    // 重写 loadFromJson 函数，从 JSON 加载食物信息
    void loadFromJson(const std::string& jsonString) override;

    void use(Player* user);

private:
    float healAmount; // 恢复的量
};

#endif // FOOD_H