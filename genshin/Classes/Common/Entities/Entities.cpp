#include "Entities.h"

// ���캯��
Entities::Entities(float health, float attack, float defence, Element element,float attackRange)
    : health(health), maxHealth(health), attack(attack), defence(defence), element(element), attackRange(attackRange),baseAttack(attack) {}


// Ĭ�Ϲ��캯��
Entities::Entities()
    : health(100), maxHealth(100), attack(10), defence(2), element(Element::FIRE), attackRange(1.5f), baseAttack(10) {}

// ����������
Entities::~Entities() {}

// ��ȡ��ǰѪ��
float Entities::getHealth() const {
    return health;
}

// ��ȡ���Ѫ��
float Entities::getMaxHealth() const {
    return maxHealth;
}

// ��ȡԪ������
Element Entities::getElement() const {
    return element;
}

// �ܵ��˺�
void Entities::takeDamage(float amount) {
    health -= amount;
    if (health < 0) health = 0;
}

// �ָ�Ѫ��
void Entities::heal(float amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// ��Ŀ������˺�
void Entities::attackTarget(Entities& target, float amount, Element element) {
    float elementModifier = calculateElementalDamageModifier(element, target.getElement());
    target.takeDamage(elementModifier * amount);
}


// ��ӡ��ǰ״̬
void Entities::printStatus() {
    CCLOG("Health: %.2f/%.2f", health, maxHealth);
    CCLOG("Attack: %.2f", attack);
    CCLOG("Defence: %.2f", defence);
    CCLOG("Element: %s", elementToString(element).c_str());
}

// ���¹�����ȴ
void Entities::updateAttackCooldown(float deltaTime) {
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;  // ������ȴʱ��
        if (currentCooldown < 0) {
            currentCooldown = 0;  // ȷ����ȴʱ�䲻��С��0
        }
    }
}

// �ж��Ƿ���Թ���
bool Entities::canAttack() const {
    return currentCooldown == 0;
}

float Entities::getAttackRange() const
{
    return attackRange;
}

bool Entities::attackInRange(Entities& target)
{
    cocos2d::Vec2 distanceVec = target.getPosition() - this->getPosition();
    float distance = distanceVec.length();
    return distance <= attackRange;
}

// ÿ֡�����߼�
void Entities::update(float deltaTime) {
    // ���¹�����ȴ
    attackCooldownAccumulator += deltaTime;
    if (attackCooldownAccumulator >= attackCooldownInterval) {
        updateAttackCooldown(attackCooldownAccumulator);  // ���¹�����ȴ
        attackCooldownAccumulator = 0.0f;  // ����ʱ��
    }

    // ����״̬Ч��
    updateStatusEffects(deltaTime);
}


// �����г�Ա�������л�Ϊ JSON ��ʽ������ JSON �ַ���
std::string Entities::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // �����ݼ��� JSON ����
    doc.AddMember("health", health, allocator);
    doc.AddMember("maxHealth", maxHealth, allocator);
    doc.AddMember("attack", attack, allocator);
    doc.AddMember("defence", defence, allocator);
    doc.AddMember("element", static_cast<int>(element), allocator);
    doc.AddMember("attackRange", attackRange, allocator); 

    // ת��Ϊ JSON �ַ���
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// �� JSON �ַ����ж�ȡ��Ա��������
void Entities::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // �� JSON ��������ȡ����
    if (doc.HasMember("health")) health = doc["health"].GetFloat();
    if (doc.HasMember("maxHealth")) maxHealth = doc["maxHealth"].GetFloat();
    if (doc.HasMember("attack")) attack = doc["attack"].GetFloat();
    if (doc.HasMember("defence")) defence = doc["defence"].GetFloat();
    if (doc.HasMember("element")) element = static_cast<Element>(doc["element"].GetInt());
    if (doc.HasMember("attackRange")) attackRange = doc["attackRange"].GetFloat();  
    baseAttack = attack;
}

// ����Ա�������л�Ϊ JSON ��ʽ�������浽����
void Entities::saveToFile(const std::string& filePath) const {
    std::string jsonString = saveToJson(); // ���� saveToJson ��ȡ JSON �ַ���
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonString;
        file.close();
        CCLOG("Entity saved to file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

// �ӱ��ض�ȡ JSON �ļ�����ȡ��Ա��������
void Entities::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf(); // ��ȡ�����ļ����ݵ� buffer
        file.close();
        std::string jsonString = buffer.str();
        loadFromJson(jsonString); // ���� loadFromJson ���� JSON ����
        CCLOG("Entity loaded from file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}


// ���״̬Ч��
void Entities::applyStatusEffect(const std::string& effect, float duration) {
    statusEffects[effect] = duration;
}

// �Ƴ�״̬Ч��
void Entities::removeStatusEffect(const std::string& effect) {
    statusEffects[effect] = 0.0f;
}

// ����״̬Ч��
void Entities::updateStatusEffects(float deltaTime) {
    for (auto& status : statusEffects) {
        if (status.second > 0.0f) {
            status.second -= deltaTime;  // ����״̬����ʱ��

            // ���ÿ��״̬��������Ӧ��Ч��
            if (status.first == "Burning" && status.second > 0.0f) {
                // ȼ��״̬��ÿ�����5���˺�
                takeDamage(5.0f);
                CCLOG("Burning effect deals 5 damage.");
            }
            else if (status.first == "Frozen" && status.second > 0.0f) {
                // ����״̬��Ŀ���޷��ж�
                CCLOG("Frozen: Target is immobilized.");
                // ������Լ��϶����߼��������״̬���޷��ƶ���������
                // ��Ҫ������ƶ����������Ҳ�����
            }
            else if (status.first == "Poisoned" && status.second > 0.0f) {
                // �ж�״̬��ÿ�����2���˺�
                takeDamage(3.0f);
                CCLOG("Poisoned effect deals 2 damage.");
            }
            else if (status.first == "Paralyzed" && status.second > 0.0f) {
                // ���״̬���޷��ж�
                CCLOG("Paralyzed: Target cannot act.");
                // ������Լ������״̬���޷��������޷��ж����߼�
                // ��Ҫ������ƶ����������Ҳ�����
            }
            else if (status.first == "Weakened" && status.second > 0.0f) {
                // ��������״̬�µĹ���������
                attack = baseAttack * 0.5f;  // ����������50%
                CCLOG("Weakened: Attack is reduced to %.2f.", attack);
            }
            else if (status.first == "Drenched" && status.second > 0.0f) {
                // ʪ��״̬������׵缼���˺���
                CCLOG("Drenched: Target is wet.");
            }
        }

        // ��״̬ʱ��Ϊ0��С��0ʱ����ʾ״̬Ч���ѽ����������ڴ˴�������
        if (status.second <= 0.0f) {
            if (status.first == "Weakened") {
                // ����״̬����ʱ�ָ�������
                attack = baseAttack;  // �ָ�ԭʼ������
                CCLOG("Weakened effect expired: Attack restored to %.2f.", attack);
            }
            CCLOG("%s effect expired.", status.first.c_str());
            // ��ɾ��״̬��ֻ�Ǽ�������ʱ��������Ч��
            // ����ѡ�����������һЩ������߼���������״̬�󴥷�����Ч������������״̬�ָ�
        }
    }
}

bool Entities::hasStatusEffect(const std::string& effect) const {
    auto it = statusEffects.find(effect);
    return it != statusEffects.end() && it->second > 0.0f;
}


void Entities::applyElementalEffects(Entities& target, Element skillElement) {
    if (target.getElement() == skillElement)    //������Ԫ����з�Ԫ��һ�£����޷���Ч
        return;
    switch (skillElement) {
        // ��ϵ���ܣ�ʹĿ��ȼ��
    case Element::FIRE:
        if (!target.hasStatusEffect("Burning")) {
            target.applyStatusEffect("Burning", 5.0f);  // �������5��
            CCLOG("Target is now Burning for 5 seconds.");
        }
        // ��ϵ���ܶԱ���Ŀ������˺����������
        if (target.hasStatusEffect("Frozen")) {
            target.removeStatusEffect("Frozen");
            target.takeDamage(15.0f);  // �������������˺�
            CCLOG("Fire skill melts frozen target and deals extra damage.");
        }
        break;

        // ��ϵ���ܣ�ʹĿ�궳��
    case Element::ICE:
        if (!target.hasStatusEffect("Frozen")) {
            target.applyStatusEffect("Frozen", 3.0f);  // ��������3��
            CCLOG("Target is now Frozen for 3 seconds.");
        }
        // ��ϵ���ܶ�ʪ��Ŀ����ǿ����Ч��
        if (target.hasStatusEffect("Drenched")) {
            target.applyStatusEffect("Frozen", 6.0f);  // ����Ч���ӳ�
            CCLOG("Ice skill enhances freeze effect on Drenched target.");
        }
        break;

        // ˮϵ���ܣ�ʹĿ��ʪ��
    case Element::WATER:
        if (!target.hasStatusEffect("Drenched")) {
            target.applyStatusEffect("Drenched", 5.0f);  // ʪ��״̬����5��
            CCLOG("Target is now Drenched for 5 seconds.");
        }
        // ˮϵ���ܶԻ�ϵĿ����ɶ����˺�
        if (target.getElement() == Element::FIRE) {
            target.takeDamage(10.0f);  // ˮϵ���ܶԻ�ϵĿ����ɶ����˺�
            CCLOG("Water skill deals extra damage to Fire target.");
        }
        break;

        // ��ϵ���ܣ���ʪ��Ŀ����ɶ����˺�
    case Element::THUNDER:
        if (target.hasStatusEffect("Drenched")) {
            target.takeDamage(20.0f);  // �׵��ʪ��Ŀ������˺�
            CCLOG("Thunder skill deals extra damage to Drenched target.");
        }
        // ��ϵ���ܿ���������״̬
        if (!target.hasStatusEffect("Paralyzed")) {
            target.applyStatusEffect("Paralyzed", 2.0f);  // ���Ч������2��
            CCLOG("Target is now Paralyzed for 2 seconds.");
        }
        break;

        // ��ϵ���ܣ����ӻ����˺�
    case Element::AIR:
        if (target.hasStatusEffect("Burning")) {
            target.takeDamage(5.0f);  // �������ʱ���ӳ������Ӷ����˺�
            target.applyStatusEffect("Burning", 2.0f);  // �������2��
            CCLOG("Wind skill increases damage on Burning target.");
        }
        break;

        // ��ϵ���ܣ����ٵ��˷���
    case Element::EARTH:
        if (!target.hasStatusEffect("Weakened")) {
            target.applyStatusEffect("Weakened", 5.0f);  // ����Ŀ��ķ���
            CCLOG("Target is now Weakened for 5 seconds.");
        }
        break;

        // ��ϵ���ܣ�ʹĿ����롰�ж���״̬
    case Element::GRASS:
        if (!target.hasStatusEffect("Poisoned")) {
            target.applyStatusEffect("Poisoned", 8.0f);  // �ж�����5��
            CCLOG("Target is now Poisoned for 5 seconds.");
        }
        break;

    default:
        break;
    }
}