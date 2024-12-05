#include "AttackSkill.h"
#include "AttackSkill.h"
#include "cocos2d.h"
#include "../../Player.h"

AttackSkill::AttackSkill(int id, const std::string& name, float cooldown, float attackPower, float range, Element element)
    : Skill(id, name, cooldown), attackPower(attackPower), range(range),element(element){}
 

// 获取攻击力
float AttackSkill::getAttackPower() const {
    return attackPower;
}

// 获取范围
float AttackSkill::getRange() const {
    return range;
}

// 激活技能
void AttackSkill::activate(Player* user, Enemy& target) {
    // 检查目标是否在范围内
    float distance = user->getPosition().distance(target.getPosition());
    if (distance > range) {
        CCLOG("AttackSkill: Target out of range for %s.", name.c_str());
        return;
    }

    // 对目标造成伤害
    user->attackTargetBySkill(target, attackPower,element);
    CCLOG("%s dealt %.2f damage to target.", name.c_str(), attackPower);
}

void AttackSkill::loadFromJson(const std::string& jsonString) {
    Skill::loadFromJson(jsonString);  // 加载基类的属性

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON for AttackSkill");
        return;
    }

    // 解析 AttackSkill 特有的属性
    if (doc.HasMember("attackPower") && doc["attackPower"].IsFloat()) {
        attackPower = doc["attackPower"].GetFloat();
    }

    if (doc.HasMember("range") && doc["range"].IsFloat()) {
        range = doc["range"].GetFloat();
    }

    if (doc.HasMember("element") && doc["element"].IsInt()) {
        element = static_cast<Element>(doc["element"].GetInt());  // 假设 Element 是一个枚举
    }
}
