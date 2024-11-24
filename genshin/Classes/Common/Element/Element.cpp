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
    throw std::invalid_argument("Invalid element string: " + str); // 异常处理，确保有效字符串
}

// 比较操作符实现
bool operator==(Element lhs, Element rhs) {
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}

bool operator!=(Element lhs, Element rhs) {
    return !(lhs == rhs);
}
