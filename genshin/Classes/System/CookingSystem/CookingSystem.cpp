#include "CookingSystem.h"
#include "json/document.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
USING_NS_CC;

CookingSystem::CookingSystem(Backpack* backpack) : backpack(backpack) {
    loadRecipesFromFile("JSON/Recipe.json");
    // ע������¼�����
    auto listener1 = cocos2d::EventListenerCustom::create("COOKING_STARTED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Cooking event received!");

        // ��������߼�
        this->setVisible(true);
        this->toStartCooking();  // �������ϵͳ�Ŀ�ʼ��⿷���
        });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);  // ����������ӵ��¼��ַ���

    // ע������¼�����
    auto listener2 = cocos2d::EventListenerCustom::create("COOKING_ENDED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Cooking event ended!");
        if (!cookingInProgress) {
            return;
        }
        cookingInProgress = false;
        this->setVisible(false);
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);  // ����������ӵ��¼��ַ���

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // ÿ֡���� update ����
        }, 0.5f, "update_key");
}


CookingSystem::~CookingSystem() {
    // ������Դ���������Ҫ�Ļ�
}

void CookingSystem::toStartCooking() {
    if (cookingInProgress) {
        return;
    }
   
    cookingInProgress = true;
    startCooking();
}

void CookingSystem::startCooking() {
    CCLOG("Cooking system initialized.");
    updateCookableRecipes();
    this->removeAllChildren();

    // ���ɽ���ҳ������
    // ����UI
    auto background = Sprite::create("Backgrounds/Cooking_Background.jpg");
    Camera* camera = Camera::getDefaultCamera();
    auto visibleSize = Director::getInstance()->getVisibleSize();
     // ��ȡ���λ�ã������ñ�����Ŀ��λ��
    Vec3 cameraPosition = camera->getPosition3D();
    Vec3 targetPosition = cameraPosition - Vec3(0, 0, 1100);  // ��ȵ���
    background->setPosition3D(targetPosition); // ���ñ���λ��
    this->addChild(background);

    // ������ͣ��Ϣ�ı���
    if (!_hoverLabelBackground) {
        _hoverLabelBackground = LayerColor::create(Color4B(0, 0, 0, 180));  // ��ɫ��͸������
        _hoverLabelBackground->setContentSize(Size(200, 100));  // ���ñ����Ĵ�С�����Ը�����Ҫ����
        background->addChild(_hoverLabelBackground,3);
    }

    // ����һ�� Label ��ʾ��ͣ��Ϣ
    if (!_hoverLabel) {
        _hoverLabel = Label::createWithSystemFont("", "Arial", 24);
        _hoverLabel->setColor(Color3B::WHITE);
        _hoverLabelBackground->addChild(_hoverLabel);  // �� Label ��ӵ�������
    }

    addHoverListener(background);

    float iconWidth = background->getContentSize().width / 5.5f;
    float iconHeight = background->getContentSize().height / 4.0f;
    float startX = background->getContentSize().width / 7.0f;
    float startY = background->getContentSize().height / 4.0f;

    int i = 0;
    for (const auto& recipe : cookableRecipes) {
        int foodId = recipe.first;
        bool canCook = recipe.second;
        // ���磬Ϊÿ��ʳ������һ����ť
        if (canCook) {
            // ����һ����ť������������⿷���
            auto button = cocos2d::MenuItemImage::create(
                "Icon/" + idToName[foodId] + ".jpg",
                "Icon/" + idToName[foodId] + ".jpg",
                [this, foodId](cocos2d::Ref* sender) {
                    this->cook(foodId, backpack);
                    this->startCooking();
                });
            // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
            float width = button->getContentSize().width;
            float height = button->getContentSize().height;

            // ����ϳ��ߵ����ű���
            float scale = 72.0f / std::max(width, height);
            button->setScale(scale);
            auto menu = cocos2d::Menu::create(button, nullptr);
            background->addChild(menu);
            menu->setPosition(cocos2d::Vec2(startX + i % 5 * iconWidth, startY + i / 5 * iconHeight));
            auto rect = Rect(cocos2d::Vec2(startX + i % 5 * (iconWidth - 72) + 36, startY + i / 5 * (iconHeight - 32) + 32), cocos2d::Size(80, 80));
            FoodHoverInfo hoverInfo;
            hoverInfo.info = "Recipe: " + idToName[foodId] + "\n" + "Raw material:";
            for (auto& it : recipes[foodId].ingredients) {
                hoverInfo.info += "\n" + idToName[it.first]+std::to_string(ingredientNums[it.first])+"/"+std::to_string(it.second);
            }
            hoverInfo.info += "/nCan cook.";
            hoverInfos[rect] = hoverInfo;
        }
        else {
            // ����һ���޷���⿵Ĳ�����ʾ
            auto foodSprite = cocos2d::Sprite::create("Icon/" + idToName[foodId] + ".jpg");
            // ��ȡ����ĵ�ǰ��Ⱥ͸߶�
            float width = foodSprite->getContentSize().width;
            float height = foodSprite->getContentSize().height;

            // ����ϳ��ߵ����ű���
            float scale = 72.0f / std::max(width, height);
            foodSprite->setScale(scale);
            foodSprite->setColor(cocos2d::Color3B(128, 128, 128));
            background->addChild(foodSprite);
            foodSprite->setPosition(cocos2d::Vec2(startX + i % 5 * iconWidth, startY + i / 5 * iconHeight)); 
            auto rect = Rect(cocos2d::Vec2(startX + i % 5 * (iconWidth - 60) + 36, startY + i / 5 * (iconHeight - 36) + 36), cocos2d::Size(80, 80));
            FoodHoverInfo hoverInfo;
            hoverInfo.info = "Recipe: " + idToName[foodId] + "\n" + "Raw material:";
            for (auto& it : recipes[foodId].ingredients) {
                hoverInfo.info += "\n" + idToName[it.first] + " " + std::to_string(ingredientNums[it.first]) + " / " + std::to_string(it.second);
            }
            hoverInfo.info += "\nCan not cook.";
            hoverInfos[rect] = hoverInfo;
        }
        i++;
    }
}

bool CookingSystem::cook(int foodId, Backpack* backpack) {
    auto recipeIt = recipes.find(foodId);
    if (recipeIt != recipes.end()) {
        const CookingRecipe& recipe = recipeIt->second;

        // ��鱳�����Ƿ����㹻�Ĳ���
        /* for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // ��ȡ��Ʒ����

            if (currentQuantity < requiredQuantity) {
                CCLOG("Not enough materials for cooking!");
                return false;  // ���ϲ��㣬�޷����
            }
        }*/

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
    auto theFood = backpack->findItemById(foodId).get();
    idToName[foodId] = theFood->getName();
    if (recipeIt != recipes.end()) {
        const CookingRecipe& recipe = recipeIt->second;

        // ��鱳�����Ƿ����㹻�Ĳ���
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // ��ȡ��Ʒ����
            ingredientNums[ingredientId] = currentQuantity;
            auto ingredient = backpack->findItemById(ingredientId).get();
            idToName[ingredientId]= ingredient->getName();
            if (currentQuantity < requiredQuantity) {
                return false;  // ���ϲ��㣬��������
            }
            auto it = idToName.find(ingredientId);

            if (it != idToName.end()) {
                auto food = backpack->findItemById(ingredientId).get();
                idToName[ingredientId] = food->getName();
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

void CookingSystem::updateCookableRecipes() {
    cookableRecipes.clear();  // ���֮ǰ��״̬

    // ��������ʳ�ײ��ж��Ƿ�������
    for (const auto& entry : recipes) {
        bool canCookRecipe = canCook(entry.first, backpack);
        cookableRecipes[entry.first] = canCookRecipe;  // ����ʳ��ID���Ƿ���������ӳ��
    }
}

// �������㲿�ֵļ�����Ϣ��ʾ
void CookingSystem::showHoverInfo(const std::string& info, const Vec2& position) {
    // ����һ������ LayerColor
    if (!_hoverLabelBackground) {
        _hoverLabelBackground = LayerColor::create(Color4B(0, 0, 0, 180));  // ��ɫ��͸������
        _hoverLabelBackground->setContentSize(Size(200, 100));  // ���ñ����Ĵ�С�����Ը�����Ҫ����
        this->addChild(_hoverLabelBackground);
    }

    // ����һ�� Label ��ʾ��ͣ��Ϣ
    if (!_hoverLabel) {
        _hoverLabel = Label::createWithSystemFont("", "Arial", 24);
        _hoverLabel->setColor(Color3B::WHITE);
        _hoverLabelBackground->addChild(_hoverLabel);  // �� Label ��ӵ�������
    }
    _hoverLabelBackground->setVisible(true);
    // ���� Label ������
    std::string hoverText = info;
    _hoverLabel->setString(hoverText);

    // ���±�����С
    _hoverLabelBackground->setContentSize(Size(_hoverLabel->getContentSize().width + 20, _hoverLabel->getContentSize().height + 20));

    // ���±������ı���λ��
    _hoverLabelBackground->setPosition(position + Vec2(50, 0)); // ����λ��
    _hoverLabel->setPosition(Vec2(_hoverLabelBackground->getContentSize().width / 2, _hoverLabelBackground->getContentSize().height / 2)); // ���־���
}

// ������ͣ��Ϣ
void CookingSystem::hideHoverInfo() {
    for (const auto& entry : hoverInfos) {
        if (entry.second.isHovering == true) {
            return;
        }
    }
    if (_hoverLabel) {
        _hoverLabel->setString("");  // ����ı�
    }
    if (_hoverLabelBackground) {
        _hoverLabelBackground->setVisible(false);  // ���ر���
    }
}

void CookingSystem::addHoverListener(Node* node) {

    auto listener = EventListenerMouse::create();

    listener->onMouseMove = [this,node](EventMouse* event) {
        // ��ȡ���λ��
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        for (auto entry : hoverInfos) {
            if (entry.first.containsPoint(cursorPosition)) {
                entry.second.isHovering = true;  // ���µ�ǰ��ͣ��Ϣ
                showHoverInfo(entry.second.info, cursorPosition);
                return;
            }
            else {
                entry.second.isHovering = false;
            }
        }

        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CookingSystem::update(float deltatime) {
    if (gaptime < 2.0f) {
        gaptime += deltatime;
        return;
    }
    hideHoverInfo();
    gaptime = 0;
}