#ifndef ENEMY_H
#define ENEMY_H

#include "../Entities.h"

class Enemy : public Entities {
private:
    int aggressionLevel;  // ���˵Ĺ����Եȼ����������˵Ĺ���Ƶ�ʻ�ǿ�ȣ�
    float detectionRadius; // ���а뾶
    int baseLevel;        // ��׼�ȼ�
    int monsterID;        // ������

public:
    Enemy(float health, Element element, int aggressionLevel, float detectionRadius, int baseLevel, int monsterID);
    Enemy();  // Ĭ�Ϲ��캯��

    int getAggressionLevel() const;
    float getDetectionRadius() const;
    int getBaseLevel() const;
    int getMonsterID() const;

    // ��������
    void attack(Entities& target) override;

    // ����AI��Ϊ
    void aiBehavior();

    // ��ӡ����״̬
    void printStatus() override;

    // ���л�Ϊ JSON �ַ���
    std::string saveToJson() const override;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;
};

#endif // ENEMY_H
