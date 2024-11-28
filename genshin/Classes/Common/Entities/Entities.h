#ifndef ENTITIES_H
#define ENTITIES_H

#include "cocos2d.h"
#include "../Element/Element.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>

class Entities : public cocos2d::Node {
protected:
    float health;          // ��ǰѪ��
    float maxHealth;       // ���Ѫ��
    float attack;
    float defence;
    Element element;       // Ԫ������

public:
    // ���캯��
    Entities(float health, float attack, float defence, Element element);

    // Ĭ�Ϲ��캯��
    Entities();

    // ����������
    virtual ~Entities();

    // ��ȡ��ǰѪ��
    float getHealth() const;

    // ��ȡ���Ѫ��
    float getMaxHealth() const;

    // ��ȡԪ������
    Element getElement() const;

    // �ܵ��˺�
    virtual void takeDamage(float amount);

    // �ָ�Ѫ��
    virtual void heal(float amount);

    // ����Ŀ��
    virtual void attack(Entities& target);

    // ��ӡ��ǰ״̬
    virtual void printStatus();

    // �����г�Ա�������л�Ϊ JSON ��ʽ������ JSON �ַ���
    virtual std::string saveToJson() const;

    // �� JSON �ַ����ж�ȡ��Ա��������
    virtual void loadFromJson(const std::string& jsonString);

    // ����Ա�������л�Ϊ JSON ��ʽ�������浽����
    virtual void Entities::saveToFile(const std::string& filePath) const;

    // �ӱ��ض�ȡ JSON �ļ�����ȡ��Ա��������
    virtual void Entities::loadFromFile(const std::string& filePath);
};

#endif // ENTITIES_H
