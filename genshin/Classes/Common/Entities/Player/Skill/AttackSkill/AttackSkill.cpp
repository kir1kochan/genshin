#include "AttackSkill.h"
#include "AttackSkill.h"
#include "cocos2d.h"
#include "../../Player.h"

AttackSkill::AttackSkill(int id, const std::string& name, float cooldown, float attackPower, float range, Element element)
    : Skill(id, name, cooldown), attackPower(attackPower), range(range),element(element){}
 

// ��ȡ������
float AttackSkill::getAttackPower() const {
    return attackPower;
}

// ��ȡ��Χ
float AttackSkill::getRange() const {
    return range;
}

// �����
void AttackSkill::activate(Player* user, Enemy& target) {
    // ���Ŀ���Ƿ��ڷ�Χ��
    float distance = user->getPosition().distance(target.getPosition());
    if (distance > range) {
        CCLOG("AttackSkill: Target out of range for %s.", name.c_str());
        return;
    }

    // ��Ŀ������˺�
    user->attackTargetBySkill(target, attackPower,element);
    CCLOG("%s dealt %.2f damage to target.", name.c_str(), attackPower);
}

void AttackSkill::loadFromJson(const std::string& jsonString) {
    Skill::loadFromJson(jsonString);  // ���ػ��������

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for AttackSkill");
        return;
    }

    // ���� AttackSkill ���е�����
    if (doc.HasMember("attackPower") && doc["attackPower"].IsFloat()) {
        attackPower = doc["attackPower"].GetFloat();
    }

    if (doc.HasMember("range") && doc["range"].IsFloat()) {
        range = doc["range"].GetFloat();
    }

    if (doc.HasMember("element") && doc["element"].IsInt()) {
        element = static_cast<Element>(doc["element"].GetInt());  // ���� Element ��һ��ö��
    }
}
