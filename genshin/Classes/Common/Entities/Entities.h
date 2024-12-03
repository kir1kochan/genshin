#ifndef ENTITIES_H
#define ENTITIES_H

#include "cocos2d.h"
#include "../Element/Element.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include <map>

class Entities : public cocos2d::Node {
protected:
    float health;          // ��ǰѪ��
    float maxHealth;       // ���Ѫ��
    float attack;
    float defence;
    Element element;       // Ԫ������

    float attackRange;     // ������Χ


    // �洢ʵ���״̬����������ж���ȼ�յ�
    std::map<std::string, float> statusEffects; // ״̬�� -> ����ʱ��

    // ������ȴ
    float attackCooldownAccumulator = 0.0f;   // �ۻ�ʱ��
    const float attackCooldownInterval=0.5f; // ÿ�θ��µ�ʱ��������λ��
    float currentCooldown = 0.0f;  // ��ǰ��ȴʱ��

public:
    // ���캯��
    Entities(float health, float attack, float defence, Element element, float attackRange);

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
    virtual void attackTarget(Entities& target, float amount, Element element);

    //����״̬
    virtual void update(float deltaTime);

    // ��ӡ��ǰ״̬
    virtual void printStatus();

    // ���¹�����ȴ
    void updateAttackCooldown(float deltaTime);

    // �ж��Ƿ���Թ���
    bool canAttack() const;

    // ��ȡ������Χ
    virtual float getAttackRange() const;

    // �ж�Ŀ���Ƿ��ڹ�����Χ��
    bool attackInRange(Entities& target);

    // ÿ֡�����߼�
    virtual void update(float deltaTime);

    // �����г�Ա�������л�Ϊ JSON ��ʽ������ JSON �ַ���
    virtual std::string saveToJson() const;

    // �� JSON �ַ����ж�ȡ��Ա��������
    virtual void loadFromJson(const std::string& jsonString);

    // ����Ա�������л�Ϊ JSON ��ʽ�������浽����
    virtual void saveToFile(const std::string& filePath) const;

    // �ӱ��ض�ȡ JSON �ļ�����ȡ��Ա��������
    virtual void loadFromFile(const std::string& filePath);

    // ���״̬Ч��
    void applyStatusEffect(const std::string& effect, float duration);

    // �Ƴ�״̬Ч��
    void removeStatusEffect(const std::string& effect);

    // ����״̬Ч��
    virtual void updateStatusEffects(float deltaTime);
};

#endif // ENTITIES_H
