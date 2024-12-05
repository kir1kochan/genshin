#include "ToolFunction.h"

Vec2 convertToSecenPoint(TMXTiledMap* map, float x, float y)
{
    //cocosĬ��Ϊ������ͼ�������ת��������������45����ͼ����Ҫ����ԭ��������ת��
    y = map->getContentSize().height - y;

    float mapOrginX = map->getPosition().x + map->getContentSize().width / 2;
    float mapOrginy = map->getPosition().y + map->getContentSize().height;

    float OA_x = (x - y) / 1.0;
    float OA_y = -(x + y) / 2.0;

    return Vec2(mapOrginX + OA_x, mapOrginy + OA_y);
}