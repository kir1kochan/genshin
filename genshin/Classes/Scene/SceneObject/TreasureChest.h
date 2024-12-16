#ifndef __TREASURECHEST_H__
#define __TREASURECHEST_H__

#include "cocos2d.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "../../Common/Backpack/Backpack.h"  


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

class TreasureChest : public cocos2d::Node {
public:
    TreasureChest();
    ~TreasureChest();

    // ���ñ����ڵ���Ʒ ID
    void setItemInChest(const cocos2d::Vec2& chestPosition, int itemID);

    // ��������
    void openChest(Backpack& backpack, const cocos2d::Vec2& chestPosition);

    // �� JSON �ļ����ر�������
    void loadFromJson(const std::string& jsonFilePath);

    // ���汦�����ݵ� JSON �ļ�
    void saveToJson(const std::string& jsonFilePath);

private:
    // �洢���б��������
    std::vector<cocos2d::Vec2> chestPositions;

    // �洢ÿ���������Ʒ ID��0 ��ʾ����Ϊ��
    std::unordered_map<cocos2d::Vec2, int> chestContents;
};

#endif  // __TREASURECHEST_H__
