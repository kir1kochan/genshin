#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

// 装备基类
class Equipment {
public:
    // 构造函数
    Equipment(const std::string& name, int power);

    // 获取装备名称
    const std::string& getName() const;

    // 获取装备的属性（攻击力或防御力）
    int getPower() const;

    // 打印装备信息（虚拟函数，允许子类重写）
    virtual void printInfo() const;

    // 将装备对象序列化为 JSON 字符串
    virtual std::string saveToJson() const;

    // 从 JSON 字符串加载装备数据
    virtual void loadFromJson(const std::string& jsonString);

    // 虚析构函数，确保派生类可以正确析构
    virtual ~Equipment() {}

private:
    std::string name;  // 装备名称
    int power;         // 装备的攻击力或防御力
};

#endif // EQUIPMENT_H