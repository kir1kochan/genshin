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
    bool isHovering;             // �Ƿ�������ͣ
    std::string info;            // ����
};


// ʳ���࣬���ڱ�ʾÿ��ʳ��
class CookingRecipe {
public:
    int foodId; // ʳ��ID
    std::unordered_map<int, int> ingredients; // ����ID������

    // ���캯��
    CookingRecipe(int foodId) : foodId(foodId) {}
    // Ĭ�Ϲ��캯��
    CookingRecipe() : foodId(-1) {}
    // ���ʳ��
    void addIngredient(int ingredientId, int quantity) {
        ingredients[ingredientId] = quantity;
    }
};

class Backpack;  // ǰ������������ѭ������

class CookingSystem : public cocos2d::Node {
public:
    CookingSystem(Backpack* backpack);  // ���캯��
    ~CookingSystem(); // ��������

    void startCooking();  // �������ϵͳ

    bool cook(int foodId, Backpack* backpack);  // ����ʳ�����ʳ��

    bool canCook(int foodId, Backpack* backpack);  // �ж��Ƿ�������ʳ��

    void loadRecipesFromJson(const std::string& jsonString);  // ��JSON�ַ�������ʳ��

    void loadRecipesFromFile(const std::string& filePath);  // �ӱ����ļ�����ʳ��

    const std::unordered_map<int, CookingRecipe>& getRecipes() const;  // ��ȡ����ʳ��



private:
    std::unordered_map<int, CookingRecipe> recipes;  // �洢ʳ�ף���Ϊʳ��ID
    std::unordered_map<int, bool> cookableRecipes;  // �洢ʳ��ID���Ƿ�����⿵�ӳ��
    std::unordered_map<int, int> ingredientNums;
    std::unordered_map<int, std::string> idToName;
    Backpack* backpack;  // ����ָ��
    float gaptime = 0;
    bool cookingInProgress=false;

    cocos2d::Label* _hoverLabel = nullptr;  // ������ʾװ���ŵ�װ���ͼ��������ı�ǩ

    cocos2d::Layer* _hoverLabelBackground = nullptr; // ��ǩ�ı���

    std::unordered_map<cocos2d::Rect, FoodHoverInfo> hoverInfos;     // �����ͣ����

    void showHoverInfo(const std::string& info, const cocos2d::Vec2& position); // ��ʾ��ͣ��Ϣ

    void updateCookableRecipes();  // ����ʳ�׿����״̬

    void hideHoverInfo();  // ������ͣ��Ϣ

    void addHoverListener(Node* node);

    void update(float deltatime);

    void toStartCooking();
};

#endif // COOKINGSYSTEM_H