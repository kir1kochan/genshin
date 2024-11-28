#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>

// ö�٣�Ԫ������
enum class Element {
    FIRE,
    WATER,
    EARTH,
    AIR,
    THUNDER,
    GRASS
};

// ��������
std::string elementToString(Element element);
Element stringToElement(const std::string& str);

// �Ƚϲ���������
bool operator==(Element lhs, Element rhs);
bool operator!=(Element lhs, Element rhs);

float calculateElementalDamageModifier(Element attackerElement, Element targetElement);
#endif // ELEMENT_H
