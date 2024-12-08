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

        // ��鱳�����Ƿ����㹻�Ĳ���
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // ��ȡ��Ʒ����

            if (currentQuantity < requiredQuantity) {
                CCLOG("Not enough materials for cooking!");
                return false;  // ���ϲ��㣬�޷����
            }
        }

        // ��⿳ɹ����Ƴ����ϲ����ʳ��
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;
            backpack->removeItem(ingredientId, requiredQuantity);  // �Ƴ�ԭ����
        }

        // ���ʳ�ﵽ����
        backpack->addItem(foodId, 1);  

        CCLOG("Cooking successful! Added food ID: %d", recipe.foodId);
        return true;
    }

    CCLOG("Recipe not found!");
    return false;  // ʳ�ײ�����
}

bool CookingSystem::canCook(int foodId, Backpack* backpack) {
    auto recipeIt = recipes.find(foodId);
    if (recipeIt != recipes.end()) {
        const CookingRecipe& recipe = recipeIt->second;

        // ��鱳�����Ƿ����㹻�Ĳ���
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // ��ȡ��Ʒ����

            if (currentQuantity < requiredQuantity) {
                return false;  // ���ϲ��㣬��������
            }
        }
        return true;  // ���㹻���ϣ���������
    }

    return false;  // ʳ�ײ�����
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

                recipes[foodId] = recipe;  // ��ʳ����ӵ�ʳ�׿�
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

    // ʹ��loadRecipesFromJson����JSON�ַ���
    loadRecipesFromJson(jsonString);
}

const std::unordered_map<int, CookingRecipe>& CookingSystem::getRecipes() const {
    return recipes;
}

std::vector<int> CookingSystem::getCanCookRecipes(Backpack* backpack) const {
    // ���������Ч�򱳰��������£��������ɻ���
    if (!cacheValid) {
        canCookCache.clear();
        for (const auto& entry : recipes) {
            if (canCook(entry.first, backpack)) {
                canCookCache.push_back(entry.first);
            }
        }
        std::sort(canCookCache.begin(), canCookCache.end());  // ��ID����
        cacheValid = true;  // ���»���״̬
    }

    return canCookCache;
}

std::vector<int> CookingSystem::getCannotCookRecipes(Backpack* backpack) const {
    // ���������Ч�򱳰��������£��������ɻ���
    if (!cacheValid) {
        cannotCookCache.clear();
        for (const auto& entry : recipes) {
            if (!canCook(entry.first, backpack)) {
                cannotCookCache.push_back(entry.first);
            }
        }
        std::sort(cannotCookCache.begin(), cannotCookCache.end());  // ��ID����
    }

    return cannotCookCache;
}

void CookingSystem::updateAvailableRecipes(Backpack* backpack) {
    // ÿ�α�������ʱ��������棬ȷ�����ݸ���
    cacheValid = false;
    cannotCookCache = getCannotCookRecipes(backpack);
    canCookCache = getCanCookRecipes(backpack);
    cacheValid = true;  // ���»���״̬
}

void CookingSystem::onEnter() {
    Node::onEnter();

    // ������ע�ᱳ�������¼�������
    backpackUpdateListener = EventListenerCustom::create("BACKPACK_UPDATED", [this](EventCustom* event) {
        // ��ȡ����ʵ��
        Backpack* backpack = static_cast<Backpack*>(event->getUserData());

        // ���� CookingSystem �Ŀ���������
        updateAvailableRecipes(backpack);
        });

    // ע���¼�������
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backpackUpdateListener, this);
}

void CookingSystem::onExit() {
    // ȡ���¼�������
    if (backpackUpdateListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(backpackUpdateListener);
        backpackUpdateListener = nullptr;
    }

    Node::onExit();
}