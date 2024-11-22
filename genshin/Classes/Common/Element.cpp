#include "element.h"
#include <stdexcept> 

// �� Element ö��ֵת��Ϊ��Ӧ���ַ���
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

// ���ַ���ת��Ϊ Element ö��ֵ
Element stringToElement(const std::string& str) {
    if (str == "Fire") return Element::FIRE;
    if (str == "Water") return Element::WATER;
    if (str == "Earth") return Element::EARTH;
    if (str == "Air") return Element::AIR;
    if (str == "Thunder") return Element::THUNDER;
    if (str == "Grass") return Element::GRASS;
    throw std::invalid_argument("Invalid element string: " + str); // �쳣����ȷ����Ч�ַ���
}

// �Ƚϲ�����ʵ��
bool operator==(Element lhs, Element rhs) {
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}

bool operator!=(Element lhs, Element rhs) {
    return !(lhs == rhs);
}
