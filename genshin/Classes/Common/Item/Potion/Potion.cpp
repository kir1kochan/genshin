#include "Potion.h"

// 构造函数
Potion::Potion(int id, const std::string& name, float effectValue)
    : Item(id, name), effectValue(effectValue) {}

// 使用药剂的效果
// 尽量可以不与食物类重复，不只是回复血量，比如 一段时间内提高攻速、一段时间内有有吸血效果 、一段时间内加快移动速度等
void Potion::use(Player& player) const {
   
}

// 重写打印信息函数
void Potion::printInfo() const {
    CCLOG("Potion Name: %s", getName().c_str());
    CCLOG("Effect Value: %.2f", effectValue);
}