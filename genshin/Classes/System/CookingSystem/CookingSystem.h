#ifndef COOKINGSYSTEM_H
#define COOKINGSYSTEM_H

#include "cocos2d.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <fstream>
#include "../../Common/Backpack/Backpack.h"
#include "../../Scene/BackpackScene/BackpackMainLayer.h"

struct FoodHoverInfo {
    bool isHovering;             // 是否正在悬停
    std::string info;            // 内容
};


// 食谱类，用于表示每个食谱
class CookingRecipe {
public:
    int foodId; // 食物ID
    std::unordered_map<int, int> ingredients; // 材料ID和数量

    // 构造函数
    CookingRecipe(int foodId) : foodId(foodId) {}
    // 默认构造函数
    CookingRecipe() : foodId(-1) {}
    // 添加食材
    void addIngredient(int ingredientId, int quantity) {
        ingredients[ingredientId] = quantity;
    }
};

class Backpack;  // 前向声明，避免循环依赖

class CookingSystem : public cocos2d::Node {
public:
    CookingSystem(Backpack* backpack);  // 构造函数
    ~CookingSystem(); // 析构函数

    void startCooking();  // 启动烹饪系统

    bool cook(int foodId, Backpack* backpack);  // 根据食谱烹饪食物

    bool canCook(int foodId, Backpack* backpack);  // 判断是否能制作食物

    void loadRecipesFromJson(const std::string& jsonString);  // 从JSON字符串加载食谱

    void loadRecipesFromFile(const std::string& filePath);  // 从本地文件加载食谱

    const std::unordered_map<int, CookingRecipe>& getRecipes() const;  // 获取所有食谱



private:
    std::unordered_map<int, CookingRecipe> recipes;  // 存储食谱，键为食物ID
    std::unordered_map<int, bool> cookableRecipes;  // 存储食谱ID和是否能烹饪的映射
    std::unordered_map<int, int> ingredientNums;
    std::unordered_map<int, std::string> idToName;
    Backpack* backpack;  // 背包指针
    float gaptime = 0;
    bool cookingInProgress=false;

    cocos2d::Label* _hoverLabel = nullptr;  // 用于显示装备着的装备和技能描述的标签

    cocos2d::Layer* _hoverLabelBackground = nullptr; // 标签的背景

    std::unordered_map<cocos2d::Rect, FoodHoverInfo> hoverInfos;     // 存放悬停内容

    void showHoverInfo(const std::string& info, const cocos2d::Vec2& position); // 显示悬停信息

    void updateCookableRecipes();  // 更新食谱可烹饪状态

    void hideHoverInfo();  // 隐藏悬停信息

    void addHoverListener(Node* node);

    void update(float deltatime);

    void toStartCooking();
};

#endif // COOKINGSYSTEM_H