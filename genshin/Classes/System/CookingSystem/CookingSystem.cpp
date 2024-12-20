#include "CookingSystem.h"
#include "json/document.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
USING_NS_CC;

CookingSystem::CookingSystem(Backpack* backpack) : backpack(backpack) {
    loadRecipesFromFile("JSON/Recipe.json");
    // 注册烹饪事件监听
    auto listener1 = cocos2d::EventListenerCustom::create("COOKING_STARTED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Cooking event received!");

        // 启动烹饪逻辑
        this->setVisible(true);
        this->toStartCooking();  // 调用烹饪系统的开始烹饪方法
        });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);  // 将监听器添加到事件分发器

    // 注册烹饪事件监听
    auto listener2 = cocos2d::EventListenerCustom::create("COOKING_ENDED_EVENT", [this](cocos2d::EventCustom* event) {
        CCLOG("Cooking event ended!");
        if (!cookingInProgress) {
            return;
        }
        cookingInProgress = false;
        this->setVisible(false);
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);  // 将监听器添加到事件分发器

    schedule([this](float deltaTime) {
        this->update(deltaTime);  // 每帧调用 update 方法
        }, 0.5f, "update_key");
}


CookingSystem::~CookingSystem() {
    // 清理资源，如果有需要的话
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

    // 生成交互页面内容
    // 创建UI
    auto background = Sprite::create("Backgrounds/Cooking_Background.jpg");
    Camera* camera = Camera::getDefaultCamera();
    auto visibleSize = Director::getInstance()->getVisibleSize();
     // 获取相机位置，并设置背景的目标位置
    Vec3 cameraPosition = camera->getPosition3D();
    Vec3 targetPosition = cameraPosition - Vec3(0, 0, 1100);  // 深度调整
    background->setPosition3D(targetPosition); // 设置背景位置
    this->addChild(background);

    // 创建悬停信息的背景
    if (!_hoverLabelBackground) {
        _hoverLabelBackground = LayerColor::create(Color4B(0, 0, 0, 180));  // 黑色半透明背景
        _hoverLabelBackground->setContentSize(Size(200, 100));  // 设置背景的大小，可以根据需要调整
        background->addChild(_hoverLabelBackground,3);
    }

    // 创建一个 Label 显示悬停信息
    if (!_hoverLabel) {
        _hoverLabel = Label::createWithSystemFont("", "Arial", 24);
        _hoverLabel->setColor(Color3B::WHITE);
        _hoverLabelBackground->addChild(_hoverLabel);  // 将 Label 添加到背景上
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
        // 例如，为每个食谱生成一个按钮
        if (canCook) {
            // 创建一个按钮，点击后调用烹饪方法
            auto button = cocos2d::MenuItemImage::create(
                "Icon/" + idToName[foodId] + ".jpg",
                "Icon/" + idToName[foodId] + ".jpg",
                [this, foodId](cocos2d::Ref* sender) {
                    this->cook(foodId, backpack);
                    this->startCooking();
                });
            // 获取精灵的当前宽度和高度
            float width = button->getContentSize().width;
            float height = button->getContentSize().height;

            // 计算较长边的缩放比例
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
            // 创建一个无法烹饪的菜谱提示
            auto foodSprite = cocos2d::Sprite::create("Icon/" + idToName[foodId] + ".jpg");
            // 获取精灵的当前宽度和高度
            float width = foodSprite->getContentSize().width;
            float height = foodSprite->getContentSize().height;

            // 计算较长边的缩放比例
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

        // 检查背包中是否有足够的材料
        /* for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // 获取物品数量

            if (currentQuantity < requiredQuantity) {
                CCLOG("Not enough materials for cooking!");
                return false;  // 材料不足，无法烹饪
            }
        }*/

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
    auto theFood = backpack->findItemById(foodId).get();
    idToName[foodId] = theFood->getName();
    if (recipeIt != recipes.end()) {
        const CookingRecipe& recipe = recipeIt->second;

        // 检查背包中是否有足够的材料
        for (const auto& ingredient : recipe.ingredients) {
            int ingredientId = ingredient.first;
            int requiredQuantity = ingredient.second;

            int currentQuantity = backpack->getItemCountById(ingredientId);  // 获取物品数量
            ingredientNums[ingredientId] = currentQuantity;
            auto ingredient = backpack->findItemById(ingredientId).get();
            idToName[ingredientId]= ingredient->getName();
            if (currentQuantity < requiredQuantity) {
                return false;  // 材料不足，不能制作
            }
            auto it = idToName.find(ingredientId);

            if (it != idToName.end()) {
                auto food = backpack->findItemById(ingredientId).get();
                idToName[ingredientId] = food->getName();
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

void CookingSystem::updateCookableRecipes() {
    cookableRecipes.clear();  // 清空之前的状态

    // 遍历所有食谱并判断是否能制作
    for (const auto& entry : recipes) {
        bool canCookRecipe = canCook(entry.first, backpack);
        cookableRecipes[entry.first] = canCookRecipe;  // 保存食谱ID和是否能制作的映射
    }
}

// 主背包层部分的技能信息显示
void CookingSystem::showHoverInfo(const std::string& info, const Vec2& position) {
    // 创建一个背景 LayerColor
    if (!_hoverLabelBackground) {
        _hoverLabelBackground = LayerColor::create(Color4B(0, 0, 0, 180));  // 黑色半透明背景
        _hoverLabelBackground->setContentSize(Size(200, 100));  // 设置背景的大小，可以根据需要调整
        this->addChild(_hoverLabelBackground);
    }

    // 创建一个 Label 显示悬停信息
    if (!_hoverLabel) {
        _hoverLabel = Label::createWithSystemFont("", "Arial", 24);
        _hoverLabel->setColor(Color3B::WHITE);
        _hoverLabelBackground->addChild(_hoverLabel);  // 将 Label 添加到背景上
    }
    _hoverLabelBackground->setVisible(true);
    // 更新 Label 的内容
    std::string hoverText = info;
    _hoverLabel->setString(hoverText);

    // 更新背景大小
    _hoverLabelBackground->setContentSize(Size(_hoverLabel->getContentSize().width + 20, _hoverLabel->getContentSize().height + 20));

    // 更新背景和文本的位置
    _hoverLabelBackground->setPosition(position + Vec2(50, 0)); // 背景位置
    _hoverLabel->setPosition(Vec2(_hoverLabelBackground->getContentSize().width / 2, _hoverLabelBackground->getContentSize().height / 2)); // 文字居中
}

// 隐藏悬停信息
void CookingSystem::hideHoverInfo() {
    for (const auto& entry : hoverInfos) {
        if (entry.second.isHovering == true) {
            return;
        }
    }
    if (_hoverLabel) {
        _hoverLabel->setString("");  // 清空文本
    }
    if (_hoverLabelBackground) {
        _hoverLabelBackground->setVisible(false);  // 隐藏背景
    }
}

void CookingSystem::addHoverListener(Node* node) {

    auto listener = EventListenerMouse::create();

    listener->onMouseMove = [this,node](EventMouse* event) {
        // 获取鼠标位置
        Vec2 cursorPosition(event->getCursorX(), event->getCursorY());
        for (auto entry : hoverInfos) {
            if (entry.first.containsPoint(cursorPosition)) {
                entry.second.isHovering = true;  // 更新当前悬停信息
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