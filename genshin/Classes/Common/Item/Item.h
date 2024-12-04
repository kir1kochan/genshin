#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <fstream>
#include <sstream>
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "cocos2d.h"
//#include "Classes/Common/Entities/Player/Player.h"
class Player;
//物品可以继承Node类，可以根据需要再改进
class Item {
public:
    // 构造函数
    Item(int id, const std::string& name);

    // 获取物品的名称
    const std::string& getName() const;

    // 获取物品的 ID
    int getId() const;

    // 打印物品信息（虚拟函数，允许子类重写）
    virtual void printInfo() const;

    // 将物品数据序列化为 JSON 字符串
    virtual std::string saveToJson() const;

    // 从 JSON 字符串加载物品数据
    virtual void loadFromJson(const std::string& jsonString);

    // 从文件加载物品数据
    virtual void loadFromFile(const std::string& filePath);

    // 将物品数据保存到文件
    virtual void saveToFile(const std::string& filePath) const;


protected:
    int id;        // 物品 ID
    std::string name;  // 物品名称
};

#endif // ITEM_H