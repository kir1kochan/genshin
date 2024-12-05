#pragma once
#ifndef _TOOL_FUNCTION_
#define _TOOL_FUNCTION_


#include "cocos2d.h"
#include "math/Vec2.h"

USING_NS_CC;
// 适用于高宽比为1：2的tiled map坐标转换函数，将地图坐标转换为世界坐标
Vec2 convertToSecenPoint(TMXTiledMap* map, float x, float y);




#endif 