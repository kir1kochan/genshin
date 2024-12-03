#ifndef POTION_H
#define POTION_H

#include "../Item.h"
#include "Classes/Common/Entities/Player/Player.h"

// 药剂类
class Potion : public Item {
public:
    // 构造函数
    Potion(int id, const std::string& name, float effectValue, const std::string& effectType);

    // 使用药剂的效果
    void use(Player& player) const;

    // 重写打印信息函数
    void printInfo() const override;

private:
    // 药剂效果值（例如：恢复血量、增加攻击力等）
    // 药效的类型可以通过id或者药品的名字确定
    float effectValue;     
    
    
};

#endif // POTION_H