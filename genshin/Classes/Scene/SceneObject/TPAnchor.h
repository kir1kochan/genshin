#ifndef __TPANCHOR_H__
#define __TPANCHOR_H__

#include "cocos2d.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "../../Common/Entities/Player/Player.h"  

// �Զ����ϣ����
namespace std {
    template <>
    struct hash<cocos2d::Vec2> {
        size_t operator()(const cocos2d::Vec2& vec) const {
            // �򵥹�ϣ���� x �� y ת��Ϊ���������
            return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
        }
    };
}

class TPAnchor : public cocos2d::Node {
public:
    TPAnchor();
    ~TPAnchor();

    // ͨ�� Player �����ý��д���
    void teleport(Player& player);
    void setPlayer(Player* player) {
        this->player = player;
    }
    // ����ĳ�� TP ��λΪ����
    void activateTPPoint(const cocos2d::Vec2& point);

    // �� JSON �ļ����� TP ��λ����
    void loadFromJson(const std::string& jsonFilePath);

    // ���� TP ��λ���ݵ� JSON �ļ�
    void saveToJson(const std::string& jsonFilePath);


    std::unordered_map<cocos2d::Vec2, bool>  gettpPointActivation() const;

private:
    // �洢���� TP ��λ����
    std::vector<cocos2d::Vec2> tpPoints;

    // �洢ÿ�� TP ��λ�ļ��������true ��ʾ���false ��ʾδ����
    std::unordered_map<cocos2d::Vec2, bool> tpPointActivation;

    std::unordered_map<cocos2d::Vec2,int> tpPointsIDs;

    Player* player;
};

#endif  // __TPANCHOR_H__
