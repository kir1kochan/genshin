#ifndef COOKINGSYSTEM_H
#define COOKINGSYSTEM_H

#include "cocos2d.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <fstream>

// 食谱类，用于表示每个食谱
class CookingRecipe {
public:
    int foodId; // 食物ID
    std::unordered_map<int, int> ingredients; // 材料ID和数量

    // 构造函数
    CookingRecipe(int foodId) : foodId(foodId) {}

    // 添加食材
    void addIngredient(int ingredientId, int quantity) {
        ingredients[ingredientId] = quantity;
    }
};

class Backpack;  // 前向声明，避免循环依赖

class CookingSystem : public cocos2d::Node {
public:
    // 构造函数
    CookingSystem();

    // 析构函数
    ~CookingSystem();

    // 启动烹饪系统
    void startCooking();

    // 根据食谱烹饪食物
    bool cook(int foodId, Backpack* backpack);

    // 判断是否能制作食物
    bool canCook(int foodId, Backpack* backpack);

    // 从JSON字符串加载食谱
    void loadRecipesFromJson(const std::string& jsonString);

    // 从本地文件加载食谱
    void loadRecipesFromFile(const std::string& filePath);

    // 获取所有食谱
    const std::unordered_map<int, CookingRecipe>& getRecipes() const;

    // 获取能制作的菜谱列表
    std::vector<int> getCanCookRecipes(Backpack* backpack) const;

    // 获取不能制作的菜谱列表
    std::vector<int> getCannotCookRecipes(Backpack* backpack) const;

    // 每次背包更新时调用，更新缓存
    void updateAvailableRecipes(Backpack* backpack);

    virtual void onEnter() override;  // 重写 onEnter 方法
    virtual void onExit() override;   // 重写 onExit 方法

private:
    std::unordered_map<int, CookingRecipe> recipes;  // 存储食谱，键为食物ID

    // 缓存
    mutable std::vector<int> canCookCache;  // 可制作的菜谱缓存
    mutable std::vector<int> cannotCookCache;  // 不能制作的菜谱缓存
    mutable bool cacheValid;  // 缓存是否有效

    cocos2d::EventListenerCustom* backpackUpdateListener;  // 事件监听器成员
};

#endif // COOKINGSYSTEM_H
