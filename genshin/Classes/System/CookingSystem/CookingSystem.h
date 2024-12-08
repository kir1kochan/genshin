#ifndef COOKINGSYSTEM_H
#define COOKINGSYSTEM_H

#include "cocos2d.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <fstream>

// ʳ���࣬���ڱ�ʾÿ��ʳ��
class CookingRecipe {
public:
    int foodId; // ʳ��ID
    std::unordered_map<int, int> ingredients; // ����ID������

    // ���캯��
    CookingRecipe(int foodId) : foodId(foodId) {}

    // ���ʳ��
    void addIngredient(int ingredientId, int quantity) {
        ingredients[ingredientId] = quantity;
    }
};

class Backpack;  // ǰ������������ѭ������

class CookingSystem : public cocos2d::Node {
public:
    // ���캯��
    CookingSystem();

    // ��������
    ~CookingSystem();

    // �������ϵͳ
    void startCooking();

    // ����ʳ�����ʳ��
    bool cook(int foodId, Backpack* backpack);

    // �ж��Ƿ�������ʳ��
    bool canCook(int foodId, Backpack* backpack);

    // ��JSON�ַ�������ʳ��
    void loadRecipesFromJson(const std::string& jsonString);

    // �ӱ����ļ�����ʳ��
    void loadRecipesFromFile(const std::string& filePath);

    // ��ȡ����ʳ��
    const std::unordered_map<int, CookingRecipe>& getRecipes() const;

    // ��ȡ�������Ĳ����б�
    std::vector<int> getCanCookRecipes(Backpack* backpack) const;

    // ��ȡ���������Ĳ����б�
    std::vector<int> getCannotCookRecipes(Backpack* backpack) const;

    // ÿ�α�������ʱ���ã����»���
    void updateAvailableRecipes(Backpack* backpack);

    virtual void onEnter() override;  // ��д onEnter ����
    virtual void onExit() override;   // ��д onExit ����

private:
    std::unordered_map<int, CookingRecipe> recipes;  // �洢ʳ�ף���Ϊʳ��ID

    // ����
    mutable std::vector<int> canCookCache;  // �������Ĳ��׻���
    mutable std::vector<int> cannotCookCache;  // ���������Ĳ��׻���
    mutable bool cacheValid;  // �����Ƿ���Ч

    cocos2d::EventListenerCustom* backpackUpdateListener;  // �¼���������Ա
};

#endif // COOKINGSYSTEM_H
