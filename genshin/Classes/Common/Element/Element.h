#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>

// 枚举：元素类型
enum class Element {
    FIRE,
    WATER,
    EARTH,
    AIR,
    THUNDER,
    GRASS
};

// 函数声明
std::string elementToString(Element element);
Element stringToElement(const std::string& str);

// 比较操作符声明
bool operator==(Element lhs, Element rhs);
bool operator!=(Element lhs, Element rhs);

float calculateElementalDamageModifier(Element attackerElement, Element targetElement);
#endif // ELEMENT_H
