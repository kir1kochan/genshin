#include "NonPlayerManager.h"

std::unordered_map<cocos2d::Vec2, SceneObject*> NonPlayerManager::getAllSceneObjects() const {
    return sceneObjects;
}

std::unordered_map<cocos2d::Vec2, Enemy*> NonPlayerManager::getAllEnemies() const {
    return enemies;
}

SceneObject* NonPlayerManager::getSceneObjectAt(const cocos2d::Vec2& position) {
    if (sceneObjects.find(position) != sceneObjects.end()) {
        return sceneObjects[position];
    }
    return nullptr;
}

Enemy* NonPlayerManager::getEnemyAt(const cocos2d::Vec2& position) {
    if (enemies.find(position) != enemies.end()) {
        return enemies[position];
    }
    return nullptr;
}

void NonPlayerManager::addSceneObject(const cocos2d::Vec2& position, SceneObject* object) {
    sceneObjects[position] = object;
}

void NonPlayerManager::addEnemy(const cocos2d::Vec2& position, Enemy* enemy) {
    enemies[position] = enemy;
}

bool NonPlayerManager::checkCollisionWithSceneObject(const cocos2d::Vec2& position, const cocos2d::Rect& rect) {
    SceneObject* object = getSceneObjectAt(position);
    if (object) {
        return rect.intersectsRect(object->getBoundingBox());
    }
    return false;
}

bool NonPlayerManager::checkCollisionWithEnemy(const cocos2d::Vec2& position, const cocos2d::Rect& rect) {
    Enemy* enemy = getEnemyAt(position);
    if (enemy) {
        return rect.intersectsRect(enemy->getBoundingBox());
    }
    return false;
}
