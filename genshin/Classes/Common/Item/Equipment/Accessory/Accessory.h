#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "../Equipment.h"
#include "Classes/Common/Element/Element.h"
class  Player;


// 饰品类，继承自装备类
class Accessory : public Equipment {
public:
    // 构造函数
    Accessory(int id, const std::string& name, int power,Element element);

    // 重写打印信息函数
    void printInfo() const override;

    Element getElement();

    // 保存为 JSON 字符串
    std::string saveToJson() const;

    // 从 JSON 字符串加载
    void loadFromJson(const std::string& jsonString);
protected:
    Element element;  // 饰品的元素属性
};

#endif // ACCESSORY_H  