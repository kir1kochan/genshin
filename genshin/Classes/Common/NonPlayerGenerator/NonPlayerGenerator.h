#ifndef NON_PLAYER_GENERATOR_H
#define NON_PLAYER_GENERATOR_H

#include "cocos2d.h"
#include "SceneObject.h"
#include "Enemy.h"
#include <unordered_map>
#include <string>

class NonPlayerGenerator {
public:
    NonPlayerGenerator() = default;
    ~NonPlayerGenerator() = default;

    // ���ɳ����������
    SceneObject* generateSceneObjectFromJson(const cocos2d::Vec2& position);

    // ���ɵ��˶���
    Enemy* generateEnemyFromJson(const cocos2d::Vec2& position);

    // �� JSON �ļ����ط�������ݣ�������������͵��ˣ�
    void loadNonPlayerDataFromJson(const std::string& jsonFilePath);

    // ����������ݣ�������������͵��ˣ����浽 JSON �ļ�
    void saveNonPlayerDataToJson(const std::string& jsonFilePath);

    // ��ȡ���г��������λ����Ϣ
    std::unordered_map<cocos2d::Vec2, std::string> getAllSceneObjectPositions() const;

    // ��ȡ���е��˵�λ����Ϣ
    std::unordered_map<cocos2d::Vec2, std::string> getAllEnemyPositions() const;

    // �����������ɵ�ʵ��
    void clear();

private:
    // �洢λ�õ� SceneObject JSON �ַ�����ӳ��
    std::unordered_map<cocos2d::Vec2, std::string> positionToSceneObjectJsonMapping;

    // �洢λ�õ� Enemy JSON �ַ�����ӳ��
    std::unordered_map<cocos2d::Vec2, std::string> positionToEnemyJsonMapping;

    // �洢�����ɵ� SceneObject ʵ��
    std::unordered_map<std::string, SceneObject*> jsonToSceneObjectMapping;

    // �洢�����ɵ� Enemy ʵ��
    std::unordered_map<std::string, Enemy*> jsonToEnemyMapping;
};

#endif // NON_PLAYER_GENERATOR_H
