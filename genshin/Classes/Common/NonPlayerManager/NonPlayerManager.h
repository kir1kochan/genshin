#ifndef NON_PLAYER_MANAGER_H
#define NON_PLAYER_MANAGER_H

#include "cocos2d.h"
#include "SceneObject.h"
#include "Enemy.h"
#include <unordered_map>
#include <string>
#include <vector>

class NonPlayerManager {
public:
    NonPlayerManager() = default;
    ~NonPlayerManager() = default;

    // ��ȡ���г��������λ����Ϣ
    std::unordered_map<cocos2d::Vec2, SceneObject*> getAllSceneObjects() const;

    // ��ȡ���е��˵�λ����Ϣ
    std::unordered_map<cocos2d::Vec2, Enemy*> getAllEnemies() const;

    // ͨ��λ�û�ȡ��������
    SceneObject* getSceneObjectAt(const cocos2d::Vec2& position);

    // ͨ��λ�û�ȡ����
    Enemy* getEnemyAt(const cocos2d::Vec2& position);

    // ��ӳ�������
    void addSceneObject(const cocos2d::Vec2& position, SceneObject* object);

    // ��ӵ���
    void addEnemy(const cocos2d::Vec2& position, Enemy* enemy);

    // ����Ƿ��볡�����巢����ײ
    bool checkCollisionWithSceneObject(const cocos2d::Vec2& position, const cocos2d::Rect& rect);

    // ����Ƿ�����˷�����ײ
    bool checkCollisionWithEnemy(const cocos2d::Vec2& position, const cocos2d::Rect& rect);

private:
    // �洢��������͵���
    std::unordered_map<cocos2d::Vec2, SceneObject*> sceneObjects;
    std::unordered_map<cocos2d::Vec2, Enemy*> enemies;
};

#endif // NON_PLAYER_MANAGER_H
