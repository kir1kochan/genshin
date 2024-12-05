#include "ToolFunction.h"

Vec2 convertToSecenPoint(TMXTiledMap* map, float x, float y)
{
    //cocos默认为正常视图坐标进行转换，但不适用于45度视图，需要换回原坐标重新转换
    y = map->getContentSize().height - y;

    float mapOrginX = map->getPosition().x + map->getContentSize().width / 2;
    float mapOrginy = map->getPosition().y + map->getContentSize().height;

    float OA_x = (x - y) / 1.0;
    float OA_y = -(x + y) / 2.0;

    return Vec2(mapOrginX + OA_x, mapOrginy + OA_y);
}