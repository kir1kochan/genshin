#ifndef __TPANCHOR_H__
#define __TPANCHOR_H__

#include "cocos2d.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Player.h"  

class TPAnchor : public cocos2d::Node {
public:
    TPAnchor();
    ~TPAnchor();

    // ͨ�� Player �����ý��д���
    void teleport(Player& player);

    // ����ĳ�� TP ��λΪ����
    void activateTPPoint(const cocos2d::Vec2& point);

    // �� JSON �ļ����� TP ��λ����
    void loadFromJson(const std::string& jsonFilePath);

    // ���� TP ��λ���ݵ� JSON �ļ�
    void saveToJson(const std::string& jsonFilePath);

private:
    // �洢���� TP ��λ����
    std::vector<cocos2d::Vec2> tpPoints;

    // �洢ÿ�� TP ��λ�ļ��������true ��ʾ���false ��ʾδ����
    std::unordered_map<cocos2d::Vec2, bool> tpPointActivation;

};

#endif  // __TPANCHOR_H__
