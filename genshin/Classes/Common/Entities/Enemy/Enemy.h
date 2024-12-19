#ifndef ENEMY_H
#define ENEMY_H

#include "../Entities.h"
#include "../../Element/Element.h"
#include <vector>
#include "../../Backpack/Backpack.h"
//#include "../Player/Player.h"

class Player;
USING_NS_CC;

class Enemy : public Entities {
private:
    int aggressionLevel;         // ���˵Ĺ����Եȼ����������˵Ĺ���Ƶ�ʻ�ǿ�ȣ�
    float detectionRadius;       // ���а뾶
    int baseLevel;               // ��׼�ȼ�
    std::string imagePath;  // �������ͼ���ļ���
    bool spriteGenerated;        // �Ƿ������ɾ���
    bool isAlive;                // �Ƿ���
    int drop;                    // ������id��������Ϊ0��
    Vector<SpriteFrame*> frames; // ����֡����
    Vector<Animate*> animates; // ��������

    Vector<SpriteFrame*> attackFrames; // ��������֡����
    Sprite* sprite; // ��ɫ����
    DrawNode* barNode;

    void drawProgressBar(cocos2d::DrawNode* barNode, float percent);
public:
    // ���캯��
    Enemy(float health, float attack, float defence, Element element, float attackRange, int aggressionLevel, float detectionRadius,
        int baseLevel, const std::string& imagePath, int drop);

    // Ĭ�Ϲ��캯��
    Enemy();

    // ��ȡ�����Եȼ�
    int getAggressionLevel() const;

    // ��ȡ���а뾶
    float getDetectionRadius() const;


    // ��ȡ��׼�ȼ�
    int getBaseLevel() const;

    // ��ȡ�����ļ���
    std::string getSpriteFilename() const;

    // ��ȡ���״̬
    bool getIsAlive() const;

    // ���ô��״̬
    void setIsAlive(bool alive);

    // �յ��˺��ĺ���
    void takeDamage(float amount) override;

    // ��������
    void attackTarget(Entities& target, float amount, Element element) override;

    // ����AI��Ϊ
    void aiBehavior(float distance, Player* player);

    // ��ӡ����״̬
    void printStatus() override;

    // ���л�Ϊ JSON �ַ���
    std::string saveToJson() const override;

    // �� JSON �ַ�����������
    void loadFromJson(const std::string& jsonString) override;

    // ������λ�õ�clone
    Enemy* clone(const cocos2d::Vec2& newPosition);

    // �����ļ������ɾ���
    cocos2d::Sprite* generateSprite() ;

    // �������Ե�����ɫ
    void applyElementColor();

    // ���þ����ļ�λ��
    void setSpriteFilename(std::string filename);

    // ���Ķ���
    Animate* getDirectionAnimate(float angle, bool isAttacking);

    // ����Ƿ������˾���
    bool isSpriteGenerated() const {
        return spriteGenerated;
    }
};

#endif // ENEMY_H
