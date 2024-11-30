#include "Entities.h"

// ���캯��
Entities::Entities(float health, float attack, float defence, Element element)
    : health(health), maxHealth(health), attack(attack), defence(defence), element(element) {}

// Ĭ�Ϲ��캯��
Entities::Entities()
    : health(100), maxHealth(100), attack(10), defence(2), element(Element::FIRE) {}

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
void Entities::attack(Entities& target, float amount, Element element) {
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
    statusEffects.erase(effect);
}

// ����״̬Ч��
void Entities::updateStatusEffects(float deltaTime) {
    // ����״̬����ʱ��
    for (auto itr = statusEffects.begin(); itr != statusEffects.end();) {
        itr->second -= deltaTime;  // ���ٳ���ʱ��
        if (itr->second <= 0) {
            // ���״̬�ѹ��ڣ��Ƴ���״̬
            itr = statusEffects.erase(itr);
        }
        else {
            ++itr;  // ���״̬���ڳ��������������һ��״̬
        }
    }
}