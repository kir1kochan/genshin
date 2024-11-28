#include "element.h"
#include <stdexcept> 

// 将 Element 枚举值转换为对应的字符串
std::string elementToString(Element element) {
    switch (element) {
    case Element::FIRE: return "Fire";
    case Element::WATER: return "Water";
    case Element::EARTH: return "Earth";
    case Element::AIR: return "Air";
    case Element::THUNDER: return "Thunder";
    case Element::GRASS: return "Grass";
    case Element::ICE: return "Ice";
    default: return "Unknown";
    }
}

// 将字符串转换为 Element 枚举值
Element stringToElement(const std::string& str) {
    if (str == "Fire") return Element::FIRE;
    if (str == "Water") return Element::WATER;
    if (str == "Earth") return Element::EARTH;
    if (str == "Air") return Element::AIR;
    if (str == "Thunder") return Element::THUNDER;
    if (str == "Grass") return Element::GRASS;
    if (str == "Ice") return Element::ICE;
    throw std::invalid_argument("Invalid element string: " + str); // 异常处理，确保有效字符串
}

// 比较操作符实现
bool operator==(Element lhs, Element rhs) {
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}

bool operator!=(Element lhs, Element rhs) {
    return !(lhs == rhs);
}

float calculateElementalDamageModifier(Element attackerElement, Element targetElement) {
    // 属性相克关系
    if (attackerElement == Element::FIRE && targetElement == Element::EARTH ||
        attackerElement == Element::WATER && targetElement == Element::FIRE ||
        attackerElement == Element::EARTH && targetElement == Element::AIR ||
        attackerElement == Element::AIR && targetElement == Element::WATER ||
        ) {
        return 1.2f;  // 火克土\水克火\土克风\风克水
    }
    if (attackerElement == targetElement) {
        return 1.0f;  // 元素相同，正常伤害
    }

    // 被克制的情况
    if ((attackerElement == Element::FIRE && targetElement == Element::WATER) ||
        (attackerElement == Element::WATER && targetElement == Element::EARTH) ||
        (attackerElement == Element::EARTH && targetElement == Element::AIR) ||
        (attackerElement == Element::AIR && targetElement == Element::FIRE)) {
        return 0.8f;  // 被克制，伤害减少
    }

    return 1.0f;  // 默认返回 1，表示没有元素相克
}

