#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "Equipment.h"

// 饰品类，继承自装备类
class Accessory : public Equipment {
public:
    // 构造函数
    Accessory(int id, const std::string& name, int power);

    // 重写打印信息函数
    void printInfo() const override;
};

#endif // ACCESSORY_H