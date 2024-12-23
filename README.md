# 项目说明文档
---
## 项目分析

### 项目背景
开放世界类 RPG 游戏（如《原神》等）凭借其高自由度、沉浸式探索以及丰富的交互玩法，近年来成为热门游戏类型之一。本项目旨在开发一个类开放世界 RPG 游戏框架，支持任务系统、互动场景、角色成长以及资源管理等核心功能，并提供基础实现。通过模块化设计和清晰的逻辑实现，开发类似原神风格的开放世界游戏。

### 项目目标
- 提供一套高扩展性的游戏框架，为未来添加新功能提供便利；
- 模拟开放世界游戏中的多种交互玩法，包括采集资源、烹饪、钓鱼、任务等；
- 引入元素克制系统，丰富战斗策略；
- 创建直观的用户界面（HUD），实时展示玩家状态信息。

### 功能需求
1. **地形与地图系统**  
   - **多样化地形**：森林、山脉、城镇、遗迹，每种地形独具挑战。  
   - **高级地图**：支持动态缩放，迷雾覆盖机制，以及传送锚点功能。  
   - **场景切换**：支持室内外地图的自由切换。  

2. **角色与任务系统**  
   - **角色自定义**：支持外观、技能、装备定制，经验升级解锁新能力。  
   - **任务系统**：包含主线任务和多种支线任务（如探索、护送、战斗等）。  

3. **战斗与装备系统**  
   - **战斗系统**：实时战斗，包含元素相克和组合技，智能 AI 提供挑战性。  
   - **装备系统**：支持武器、防具和特殊物品的收集与升级。

4. **环境互动**  
   - 支持采集、钓鱼、烹饪等玩法，增强探索体验。

5. **可选扩展功能**  
   - 多人协作模式、动态天气系统以及原创音效与配乐。

  ---

## 项目整体框架设计
该项目基于 Cocos2d-x 引擎开发，旨在创建一个集角色管理、任务系统和交互式场景为一体的类原神风格RPG（角色扮演）简易游戏。项目采用模块化设计，将不同功能划分为独立系统，降低耦合度，提升代码复用性与可维护性。

**主要模块概览**:
- Common（通用模块）提供角色管理、技能系统、物品系统等核心游戏元素。
- Backpack：背包管理系统。
- Element：元素系统的实现逻辑
- Scene（场景类）管理不同场景的元素和交互层的实现。
- BackpackScene：背包和技能管理场景。
- SceneObject：场景交互对象（如宝箱、传送点等）。
- System（系统类）：实现核心功能系统，如烹饪、护送任务、钓鱼等交互式玩法。
- UI（用户界面）：负责游戏内的 HUD（血条、技能栏）等前端展示逻辑。
- JSON（配置文件）：存放任务、食谱等配置信息，支持动态加载游戏数据和存放游戏数据。

---

## 核心功能解析
### 元素系统
#### 概述
- 元素系统（Element System）是游戏中角色技能、攻击和防御的基础机制，涉及属性克制与抗性计算。该系统旨在通过不同的元素类型，为战斗系统添加策略性，使不同角色和怪物之间的交互更加丰富。
####核心功能与实现要点
- Element 系统的设计简洁，主要以 enum class 枚举定义七种基础元素类型，并通过函数实现元素克制关系和转换功能。
#####元素枚举的设计
- enum class Element { FIRE, WATER, EARTH, AIR, THUNDER, GRASS, ICE };
#####主要功能函数
- elementToString：将枚举转换为字符串表示。
- stringToElement：将字符串解析回 Element 枚举。
- calculateElementalDamageModifier：根据攻击者和目标的元素计算伤害修正系数，实现元素克制逻辑。

### 背包系统
#### 概述
- 背包系统是游戏的重要组成部分之一，负责管理玩家的物品、装备及技能等资源，提供存储、检索、删除和使用物品的功能。在该项目中，背包系统围绕 Backpack 类展开，通过交互界面展示物品状态，确保游戏过程中的物品管理直观、易用。
#### 主要类结构与职责
- Backpack作为背包的核心逻辑类，负责存储和管理物品。Backpack功能实现的核心成员变量为std::unordered_map<int, int> items，用于负责存储和管理物品；主要方法有：
- void addItem(int itemId, int count)：添加指定数量的物品到背包。
- void removeItem(int itemId, int count)：移除物品，数量不足则删除该物品。
- int getItemCountById(int itemId)：获取指定物品的数量。
- BackpackMainLayer 类管理背包 UI 及物品展示，处理玩家对背包的交互,主要使用使用 Sprite 显示物品图标，使用监听鼠标悬停事件，展示物品的详细信息。BackpackMainLayer 类的主要方法为：
- void switchToBackpackLayer()：切换到背包界面。
- void updatePlayerData()：刷新背包数据，更新界面显示。
- void refreshEquipmentIcons()：更新装备物品的显示状态。
#### 功能流程介绍
##### 物品拾取与添加流程
- 当玩家在场景中拾取物品时，调用 Backpack::addItem 方法，将物品添加到背包。
- 如果该物品在背包中已存在，则叠加数量；如果不存在，则创建新的条目。
- UI 自动刷新，显示物品数量变化。
##### 物品使用与移除流程
- 玩家在背包界面点击使用物品后，触发removeItem方法，减少物品数目；
- 当物品数目减少为0时，从 items 容器中移除该物品；
- UI 自动刷新，显示物品数量变化或者移除物品。
##### 交互实现逻辑
**鼠标悬停提示：**
- 使用 addHoverListenerForIcons 方法为物品图标添加悬停监听器；
- 鼠标悬停时调用 showHoverInfo 方法，显示物品的详细属性（如攻击力、稀有度等）；
- 离开悬停区域后调用 hideHoverInfo 隐藏提示信息；
**快捷栏物品移动与使用**
- 玩家可以拖动物品到快捷栏，或直接在快捷栏装备/卸载物品；
- 使用快捷键映射可以快速使用快捷栏中的技能或消耗品。

### 烹饪系统
#### 概述
- 烹饪系统（Cooking System）是游戏中核心的生存和成长机制之一，通过收集食材并组合加工，玩家可以制作各种食物和药剂，提升角色属性或恢复状态。系统包含食谱解析、食材管理和动态烹饪流程，极大地增强了游戏的探索与成长体验。
#### CookingRecipe类
- 表示单个食谱，存储食物 ID 及其所需食材。
- 其中数据成员int foodId表示成品食物 ID，std::unordered_map<int, int> ingredients存储制作该食品需要的食材及数量；
- 主要方法void addIngredient(int ingredientId, int quantity)为向食谱中添加食材。
#### 主要类与职责
- CookingSystem类是烹饪系统的核心逻辑类，负责烹饪流程的整体控制，包括食谱解析、烹饪判定和结果反馈。
- CookingSystem类核心数据成员分别为std::unordered_map<int, CookingRecipe> recipes，食谱库，负责存储食谱；std::unordered_map<int, bool> cookableRecipes，存储食谱对应的可烹饪状态；Backpack* backpack，玩家背包指针。
- 主要方法包括：
- void startCooking()：启动烹饪界面，显示所有可烹饪的食谱；
- bool cook(int foodId)：尝试烹饪指定食物，成功则消耗食材并获得；
- void loadRecipesFromFile(const std::string& path)：从 JSON 文件加载食谱；
- void updateCookableRecipes()：根据背包中的食材，更新哪些食谱可以烹饪。
#### 实现逻辑
- 玩家与烹饪台交互，触发烹饪界面弹出；
- 系统加载配方数据，遍历玩家背包，更新可制作的配方状态；
- 鼠标悬停在配方上，显示需要材料和当前材料数量对比；
- 点击配方按钮，系统检查材料，如果材料不足，弹出提示；如果材料满足，扣除材料并生成食物，放入背包。
- 烹饪结束后，界面关闭，返回主游戏场景。

### 钓鱼系统
#### 概述
- 钓鱼系统作为游戏中的一个休闲交互功能，为玩家提供了额外的资源获取方式。玩家可以在特定的钓鱼点参与钓鱼小游戏，通过时机判断与反应操作，决定是否成功钓到鱼或其他物品。
- 钓鱼系统不仅增强了游戏的趣味性，还增加了玩家探索和资源管理的多样性。钓鱼成功与否主要取决于玩家的反应速度，增加了实时交互的乐趣。
#### 主要类与职责
- FishingSystem 负责钓鱼小游戏的整体逻辑控制，包括启动钓鱼、动态更新进度条，以及成功或失败的判断与回调机制。
- FishingSystem类数据成员及其作用主要为：
- float fishingSuccessTime:成功钓鱼的时间窗口起始时间
- float elapsedTime:钓鱼小游戏经过的时间 
- bool keyPressed:标识玩家是否按下空格键 
- bool fishingInProgress: 标识钓鱼是否正在进行 
- std::function<void(bool)> fishingResultCallback:钓鱼结果回调函数 
- DrawNode* progressBarNode: 红色背景进度条 
- DrawNode* greenBarNode: 绿色成功区域条 
- Sprite* backgroundBar: 钓鱼界面背景 
- Label* fishingLabel: 提示信息标签
**主要方法包括**
- void startFishing(Scene* fishingScene): 启动钓鱼小游戏，生成UI界面，设置随机钓鱼时间窗口，同时监听键盘输入判断结果；
- void onKeyPress(EventKeyboard::KeyCode keyCode, Event* event)：按下空格键时，判断是否处于绿色条范围内即钓鱼，持续时间处于窗口时间内。若处于窗口时间内，即判定为钓鱼成功。
- void setOnFishingResultCallback(std::function<void(bool)> callback) 外部类可调用该方法注册回调函数，实现钓鱼成功或失败后触发的逻辑。
#### 实现逻辑
- 玩家靠近钓鱼点，按下交互键触发 FISHING_STARTED_EVENT，启动钓鱼；
- 钓鱼界面包含动态红绿进度条，绿色条不断向左移动；若玩家在绿色窗口内按下按键，则钓鱼成功，物品加入背包；若超出窗口或在有效时间内未按键，则钓鱼失败。
- 钓鱼结束后，界面自动关闭，恢复到普通场景状态。

### 场景交互对象系统
#### 概述
- 场景交互对象系统是游戏中玩家与环境交互的关键组成部分。它负责管理和触发各种场景中的可交互对象，例如传送点、宝箱以及采集点等。通过该系统，玩家可以与世界中的物体进行互动，从而触发事件、获得物品或切换场景。
#### 主要类与方法
- SceneObject 作为所有交互对象的基类，定义了交互对象的基本属性和交互逻辑，支持不同类型的交互方式（如采摘、钓鱼等）。
- FishingSystem 主要数据成员及其作用
- ObjectType type：存储交互对象类型（采摘、烹饪、钓鱼等）
- std::string imagePath：存储交互对象的图像路径 
- bool spriteGenerated：判断交互对象是否生成了精灵 
- std::vector<int> itemIds：掉落的物品或采集物品等的ID列表
**主要方法包括**
- void interactWithPlayer(Backpack* backpack)：根据交互对象的类型，执行不同的交互逻辑（如采集物品、触发烹饪界面或钓鱼小游戏）
- void loadFromJson (const std::string& jsonString)：从 JSON 文件中加载交互对象的类型、位置和物品掉落信息。
- SceneObject* clone(const Vec2& position) const：用于生成多个相同类型的交互对象，以便在不同位置放置相同类型的场景物体。
#### 实现逻辑
- 当玩家靠近场景交互对象时，触发 interactWithPlayer 方法。
- 系统检测对象类型（采摘、钓鱼、烹饪等），根据类型执行不同逻辑。
- 交互对象完成预设逻辑后，等待下一次触发。
### HUD系统
#### 概述
- HUD（Heads-Up Display）系统负责游戏中玩家状态（如血量、体力和技能冷却）的可视化展示。HUD 提供直观的反馈，帮助玩家实时掌握角色状态及技能使用情况。
#### 核心类与方法解析
- Hud类是HUD系统的核心，负责初始化血条、体力条和技能栏的 UI 组件，并在游戏运行过程中实时更新。
- Hud的数据成员包括：
- cocos2d::DrawNode* healthBarNode：血条节点 
- cocos2d::DrawNode* staminaBarNode： 体力条节点 
- std::vector<cocos2d::DrawNode*> skillBarNode： 技能栏节点 
- std::vector<cocos2d::ProgressTimer*> skillCooldownBars： 技能冷却进度条 
- Player* player：玩家指针，用于获取玩家实时状态
- float barWidth：血条与体力条的宽度
- float barHeight：进度条的高度
- Hud的核心方法包括：
- Hud(Player* player)：含参构造函数，在创建时生成血条和体力条节点。
- void equipSkill(int skillSlot, std::string skillName)：通过 equipSkill 将技能图标绑定至技能栏槽位，并启动冷却进度条更新冷却状态。
- void useSkill(int skillSlot, float cdTime)：触发技能后，启动冷却计时，并动态调整冷却条长度，直观展示剩余冷却时间。
#### 实现逻辑
- 游戏启动时，HUD 系统初始化血条、体力条和技能栏，设置玩家状态的默认 UI。
- 在游戏主循环中，每帧调用 update 方法，读取玩家血量和体力，并更新对应进度条的长度。
- 当玩家学习新技能或切换技能时，调用equipSkill将新技能绑定到技能栏槽位，加载技能图标。
- 玩家释放技能后，HUD启动冷却进度条，技能栏UI随冷却时间逐渐减少，技能冷却完成后进度条自动消失。

---

## 类及其功能
本小节将介绍各个类，已经在上一小节中详细介绍的实体类不在这里赘述。
- **Entities**: 负责定义游戏中的玩家、敌人等动态实体的基本属性（如生命值、位置）和行为逻辑（如回复生命、攻击等）。
- **Enemy**：游戏中敌人的具体实现，继承自Entities类，用于定义敌人的属性和行为逻辑。该类包括敌人的生命值、攻击力、元素属性，以及AI行为（如自动攻击、巡逻）。此外，Enemy类支持与玩家交互的触发机制，如追踪玩家或执行特定技能。
- **NPC**：游戏中非玩家、怪物角色的实现，用于加载 NPC 的形象、设置位置，并触发对话功能。通过对话文本与玩家互动，提供剧情或任务提示。同时支持动态加载资源，并使用 Cocos2d-x 的 Label 显示对话框内容。
- **Player**：游戏中的核心角色类，负责管理玩家的属性（如生命值、体力、技能等）、与场景的交互，以及状态更新。它整合了角色的战斗、移动和任务系统功能，是整个游戏逻辑的关键驱动。
- **EventManager**:一个负责管理和分发游戏中各种事件的核心类。它用于协调不同系统或模块之间的通信，解耦模块之间的依赖关系。
- **ButtonEventManager**: 管理游戏中按钮事件的类，负责将按钮与具体功能绑定，实现用户输入与游戏功能之间的交互。它主要管理按钮点击、长按等事件的触发逻辑，简化了事件监听的实现。
- **InventoryMouseEventManager**:用于管理背包系统中鼠标交互事件的类，主要负责处理玩家在背包界面中的鼠标操作，包括点击、拖拽和悬停等操作。
- **KeyboardEventManager**:管理键盘事件的核心类，主要用于处理玩家通过键盘与游戏进行的交互操作。它负责监听按键事件，并将事件与具体的游戏功能（如角色移动、技能释放、菜单导航等）绑定起来。
- **MainGameMouseEventManager**: 用于管理游戏主场景中鼠标事件的核心类，负责处理鼠标在主场景中与游戏对象的交互逻辑。通过鼠标事件管理，玩家可以通过点击或移动鼠标与游戏中的 NPC、物品、敌人、地图等进行交互。
- **Item**: 物品系统的核心组件，用于表示游戏中的各种物品，包括装备、消耗品、任务物品等。游戏世界中物品的抽象表示，支持属性定义、序列化、交互和展示等功能。
- **Equipment**: 所有装备物品的基类，用于表示可装备的物品。
- **Accessory**: 装备系统的一部分，用于表示可以装备的饰品类物品。
- **Armor**: 游戏装备系统中的组成部分，用于表示可装备的护甲物品。
- **Food**: 游戏物品系统的一部分，用于表示玩家在游戏中可以拾取、使用的食物物品。
- **Potion**: 主要用于表示玩家可以在战斗或探索中使用的药剂。
- **Weapon**:表示玩家或敌人可以装备的武器，武器通常会影响攻击速度、攻击范围等战斗相关的属性。
- **BlockManager**: 管理游戏中的区块（Block）及其行为。
- **SpiritManager**: 管理精灵实体的系统类。
- **StoryManager**: 管理游戏剧情和故事进程的核心模块，负责控制游戏的主线剧情、分支任务、对话和触发事件的执行。
- **Skill**: 技能系统的基础类，为所有具体技能提供通用接口和核心功能。
- **AttackSkill**: 实现攻击技能的逻辑，包括攻击属性、攻击范围、冷却时间等。
- **HealSkill**: 实现治疗类技能的功能，负责恢复玩家或目标单位的生命值；
- **ShieldSkill**: 实现防护类技能的功能，为目标单位增加护盾值。
- **TPAnchor** 交互系统中的传送点（Teleport Anchor）管理类，负责实现游戏中玩家传送功能的核心逻辑。
- **TreasureChest**: 宝箱管理类，负责实现宝箱物品存储、玩家交互拾取等功能。

---

## 游戏基本流程（项目总体运行逻辑）
### 游戏开始
- **启动游戏**
- 玩家通过启动界面选择“开始游戏”。
- 游戏加载玩家存档数据，包括背包物品、当前技能和装备、已完成或进行中的任务状态等。
- **玩家角色初始化**
- 系统生成玩家的初始位置、血量、体力等基本状态。
- 加载当前场景，包括场景交互对象（如传送点、宝箱、采集点等）。
  
### 探索与资源管理
- **场景探索**
- 玩家在地图中自由探索，发现场景中的交互对象（SceneObject），如食物、传送点、宝箱等。
- **资源管理**
- 玩家通过采集、战斗、完成任务等方式获得资源。
- 背包系统负责动态管理物品，并提供使用或装备功能。

### 任务与战斗
- **任务系统**
- 玩家可以在场景中与 NPC 互动或触发特定事件来接受任务。
- 任务类型包括 等。
- **战斗系统**
- 玩家通过技能系统与敌人交战。每个技能有对应的冷却时间和消耗的体力；战斗中涉及元素克制关系。

### 互动玩法
- **烹饪系统**
- 玩家可通过在烹饪点触发烹饪界面。制作食物时系统从背包中检查所需材料，判断是否可以制作对应食物，制作成功后，将食物添加至背包。
- **钓鱼系统**
- 玩家在钓鱼点触发钓鱼界面，需在特定时间窗口按下按键以完成钓鱼，钓鱼成功后，获得随机物品（如鱼或稀有资源）。

### 进阶机制与成长
- **技能与装备**
- 玩家可以在背包或技能界面查看已解锁的技能和装备。
- 技能冷却时间和装备属性直接影响战斗策略。
- **任务及战斗奖励**
- 玩家完成任务、击败敌人后，获得奖励，包括货币、资源、经验等。

### 开放世界探索
- **场景交互**
- 玩家可以在地图中自由切换不同区域，通过传送点快速移动，探索新区域时，可以解锁更多任务和资源。
- **动态事件**
- 场景中可能随机生成动态事件（如随机任务、稀有敌人出现、隐藏宝箱解锁等）。

---

## 使用的C++特性
### STL容器和算法
- 标准模板库（STL）提供了高效的数据结构和算法，如 vector、unordered_map、find 等。在本项目中，使用了许多容器来存储信息，例如，在背包系统中，使用了 unordered_map<int, int> 存储物品和数量，其中键为物品ID，值为数量。 
- std::unordered_map<int, int> items;
- 使用容器可以实现高效的查找和操作支持，同时std::find等函数提供了通用的集合操作，避免手动实现重复逻辑，以及stl容器提供了迭代器操作，简化了遍历操作。

### 面向对象编程（OOP）
- 本项目广泛使用了 OOP 的特性，包括封装、继承和多态。
- 封装：各系统类（如 CookingSystem、FishingSystem）将功能与数据逻辑封装在类中，通过公共接口对外提供功能。
- 继承：游戏场景中的交互对象（如宝箱、传送点）均继承自基类 SceneObject，并实现其特定功能，如 TreasureChest 和 TPAnchor。
- 多态: 使用基类指针实现对不同类型交互对象的统一操作。
- 这样做的优点是代码逻辑清晰，符合游戏中对象和功能的分层设计，同时继承和多态使得对游戏对象的扩展更加便捷。

### 智能指针
- 智能指针（如 std::shared_ptr、std::unique_ptr）通过 RAII 管理动态内存，避免手动释放内存导致的资源泄漏。背包系统和技能系统中，使用 std::shared_ptr 管理物品和技能对象的生命周期。
- 例如角色的各类信息存储：
  ```
  std::shared_ptr<Weapon> weapon; 
  std::shared_ptr<Armor> armor; 
  std::shared_ptr<Accessory> accessory; 
  std::vector<std::shared_ptr<Skill>> unlockedSkills;
  std::vector<std::shared_ptr<Skill>> skillBar;
  ```
- 使用智能指针，避免手动 delete 操作，提高代码安全性，减少悬空指针和内存泄漏的风险。

### 异常处理
- 异常处理用于捕获并处理程序运行中的错误，确保程序的健壮性。在元素系统中，字符串向枚举类型转换时检测非法输入并抛出异常。例如：
   ```
     Element stringToElement(const std::string& str) {     
        if (str == "Fire") return Element::FIRE;
        if (str == "Water") return Element::WATER;
        if (str == "Earth") return Element::EARTH;
        if (str == "Air") return Element::AIR;
        if (str == "Thunder") return Element::THUNDER;
        if (str == "Grass") return Element::GRASS;
        if (str == "Ice") return Element::ICE;
        throw std::invalid_argument("Invalid element string: " + str); 
    }
  ```
- 避免非法信息导致的程序崩溃，同时提供明确的错误信息，便于调试。

### 函数重载与操作符重载
- 函数重载允许在同一作用域内定义具有相同名称但参数列表不同的函数；操作符重载允许为自定义类型定义运算符行为。在各个类中，大多同时实现了无参和含参构造函数，可以根据实际传入参数情况调用具体函数。同时，在元素系统中重载了 == 和 != 运算符，方便元素比较。例如：
```
bool operator!=(Element lhs, Element rhs) {
    return !(lhs == rhs);
}
```
 ---
# 项目开发环境说明
- 操作系统：    Windows 11 64-bit
- 集成开发环境：Visual Studio 2022
- 编译语言：    C++ 
- C++标准：     C++11
- 游戏引擎：    cocos2d-x 3.17.2
- 地图制作：    Tiled 1.11.0
- 版本控制：    Git（GitHub 托管）
