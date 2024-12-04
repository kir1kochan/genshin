#include "Element.h"
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
    case Element::ICE: return "Ice";
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
    if (str == "Ice") return Element::ICE;
    throw std::invalid_argument("Invalid element string: " + str); // �쳣����ȷ����Ч�ַ���
}

// �Ƚϲ�����ʵ��
bool operator==(Element lhs, Element rhs) {
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}

bool operator!=(Element lhs, Element rhs) {
    return !(lhs == rhs);
}

float calculateElementalDamageModifier(Element attackerElement, Element targetElement) {
    // ���Կ��ƹ�ϵ
    if ((attackerElement == Element::FIRE && targetElement == Element::GRASS) ||
        (attackerElement == Element::WATER && targetElement == Element::FIRE) ||
        (attackerElement == Element::EARTH && targetElement == Element::THUNDER) ||
        (attackerElement == Element::AIR && targetElement == Element::ICE) ||
        (attackerElement == Element::THUNDER && targetElement == Element::WATER) ||
        (attackerElement == Element::GRASS && targetElement == Element::EARTH) ||
        (attackerElement == Element::ICE && targetElement == Element::THUNDER)) {
        return 1.2f;  // ���ƣ��˺�����
    }

    // ���Ա����ƹ�ϵ
    if ((attackerElement == Element::FIRE && targetElement == Element::WATER) ||
        (attackerElement == Element::WATER && targetElement == Element::THUNDER) ||
        (attackerElement == Element::EARTH && targetElement == Element::GRASS) ||
        (attackerElement == Element::AIR && targetElement == Element::EARTH) ||
        (attackerElement == Element::THUNDER && targetElement == Element::ICE) ||
        (attackerElement == Element::GRASS && targetElement == Element::FIRE) ||
        (attackerElement == Element::ICE && targetElement == Element::AIR)) {
        return 0.8f;  // �����ƣ��˺�����
    }

    // ������ͬ
    if (attackerElement == targetElement) {
        return 1.0f;  // Ԫ����ͬ�������˺�
    }

    return 1.0f;  // Ĭ�Ϸ��������˺�
}

