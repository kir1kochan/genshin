#include "CookingSystem.h"
#include "rapidjson/document.h"
#include "Backpack.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

CookingSystem::CookingSystem() : cacheValid(false) {}

CookingSystem::~CookingSystem() {
    if (backpackUpdateListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(backpackUpdateListener);
    }
}

void CookingSystem::startCooking() {
    CCLOG("Cooking system initialized.");
}

bool CookingSystem::cook(int foodId, Backpack* backpack) {
    auto recipeIt = recipes.find(foodId);
    if (recipeIt != recipes.end()) {
        const CookingRecipe& recipe = recipeIt->second;

        // 检查背包中是否有足够的材料
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // 获取物品数量

            if (currentQuantity < requiredQuantity) {
                CCLOG("Not enough materials for cooking!");
                return false;  // 材料不足，无法烹饪
            }
        }

        // 烹饪成功，移除材料并添加食物
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;
            backpack->removeItem(ingredientId, requiredQuantity);  // 移除原材料
        }

        // 添加食物到背包
        backpack->addItem(foodId, 1);  

        CCLOG("Cooking successful! Added food ID: %d", recipe.foodId);
        return true;
    }

    CCLOG("Recipe not found!");
    return false;  // 食谱不存在
}

bool CookingSystem::canCook(int foodId, Backpack* backpack) {
    auto recipeIt = recipes.find(foodId);
    if (recipeIt != recipes.end()) {
        const CookingRecipe& recipe = recipeIt->second;

        // 检查背包中是否有足够的材料
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // 获取物品数量

            if (currentQuantity < requiredQuantity) {
                return false;  // 材料不足，不能制作
            }
        }
        return true;  // 有足够材料，可以制作
    }

    return false;  // 食谱不存在
}

void CookingSystem::loadRecipesFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        CCLOG("Error parsing JSON string.");
        return;
    }

    if (doc.HasMember("recipes") && doc["recipes"].IsArray()) {
        const rapidjson::Value& recipeArray = doc["recipes"];

        for (rapidjson::SizeType i = 0; i < recipeArray.Size(); ++i) {
            const rapidjson::Value& recipeObj = recipeArray[i];

            if (recipeObj.HasMember("foodId") && recipeObj["foodId"].IsInt()) {
                int foodId = recipeObj["foodId"].GetInt();
                CookingRecipe recipe(foodId);

                if (recipeObj.HasMember("ingredients") && recipeObj["ingredients"].IsObject()) {
                    const rapidjson::Value& ingredients = recipeObj["ingredients"];
                    for (auto& ingredient : ingredients.GetObject()) {
                        int ingredientId = std::stoi(ingredient.name.GetString());
                        int quantity = ingredient.value.GetInt();
                        recipe.addIngredient(ingredientId, quantity);
                    }
                }

                recipes[foodId] = recipe;  // 将食谱添加到食谱库
            }
        }
    }
}

void CookingSystem::loadRecipesFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        CCLOG("Failed to open file: %s", filePath.c_str());
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonString = buffer.str();

    // 使用loadRecipesFromJson解析JSON字符串
    loadRecipesFromJson(jsonString);
}

const std::unordered_map<int, CookingRecipe>& CookingSystem::getRecipes() const {
    return recipes;
}

std::vector<int> CookingSystem::getCanCookRecipes(Backpack* backpack) const {
    // 如果缓存无效或背包发生更新，重新生成缓存
    if (!cacheValid) {
        canCookCache.clear();
        for (const auto& entry : recipes) {
            if (canCook(entry.first, backpack)) {
                canCookCache.push_back(entry.first);
            }
        }
        std::sort(canCookCache.begin(), canCookCache.end());  // 按ID排序
        cacheValid = true;  // 更新缓存状态
    }

    return canCookCache;
}

std::vector<int> CookingSystem::getCannotCookRecipes(Backpack* backpack) const {
    // 如果缓存无效或背包发生更新，重新生成缓存
    if (!cacheValid) {
        cannotCookCache.clear();
        for (const auto& entry : recipes) {
            if (!canCook(entry.first, backpack)) {
                cannotCookCache.push_back(entry.first);
            }
        }
        std::sort(cannotCookCache.begin(), cannotCookCache.end());  // 按ID排序
    }

    return cannotCookCache;
}

void CookingSystem::updateAvailableRecipes(Backpack* backpack) {
    // 每次背包更新时，清除缓存，确保数据更新
    cacheValid = false;
    cannotCookCache = getCannotCookRecipes(backpack);
    canCookCache = getCanCookRecipes(backpack);
    cacheValid = true;  // 更新缓存状态
}

void CookingSystem::onEnter() {
    Node::onEnter();

    // 创建并注册背包更新事件监听器
    backpackUpdateListener = EventListenerCustom::create("BACKPACK_UPDATED", [this](EventCustom* event) {
        // 获取背包实例
        Backpack* backpack = static_cast<Backpack*>(event->getUserData());

        // 更新 CookingSystem 的可制作菜谱
        updateAvailableRecipes(backpack);
        });

    // 注册事件监听器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backpackUpdateListener, this);
}

void CookingSystem::onExit() {
    // 取消事件监听器
    if (backpackUpdateListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(backpackUpdateListener);
        backpackUpdateListener = nullptr;
    }

    Node::onExit();
}