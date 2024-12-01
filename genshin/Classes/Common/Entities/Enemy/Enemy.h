#ifndef ENEMY_H
#define ENEMY_H

#include "../Entities.h"

class Enemy : public Entities {
private:
    int aggressionLevel;  // ���˵Ĺ����Եȼ����������˵Ĺ���Ƶ�ʻ�ǿ�ȣ�
    float detectionRadius; // ���а뾶
    int baseLevel;        // ��׼�ȼ�
    std::string spriteFilename;  // �������ͼ���ļ���
    bool spriteGenerated; // �Ƿ������ɾ���

public:
    Enemy(float health, Element element, int aggressionLevel, float detectionRadius, int baseLevel, const std::string& spriteFilename);
    Enemy();  // Ĭ�Ϲ��캯��

    int getAggressionLevel() const;
    float getDetectionRadius() const;
    int getBaseLevel() const;
    std::string getSpriteFilename() const;

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

    // ������λ�õ�clone
    Enemy* clone(const cocos2d::Vec2& newPosition);

    // �����ļ������ɾ���
    cocos2d::Sprite* generateSprite() const;

    // ����Ƿ������˾���
    bool isSpriteGenerated() const {
        return spriteGenerated;
    }
};

#endif // ENEMY_H