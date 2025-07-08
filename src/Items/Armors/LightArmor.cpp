#include "LightArmor.h"

LightArmor::LightArmor(QObject *parent)
    : Armor(parent, ":/Items/Armors/Light/light.png")
{
}

void LightArmor::mountToParent()
{
    // 首先调用基类的通用逻辑
    Armor::mountToParent();
    // 然后在这里设置轻甲专属的穿戴位置
    // 例如，让它比默认盔甲稍微往上和往右一点
    setPos(0, -90);
    setScale(0.65); // 也可以微调大小
}

void LightArmor::unmount()
{
    // 首先调用基类的通用逻辑
    Armor::unmount();
    // 然后在这里设置轻甲专属的掉落位置和外观
    // 例如，让它掉在地上时大一点，方便拾取
    setScale(0.25);
    // 调整贴图位置，使其看起来是“立”在地面上
    if (pixmapItem != nullptr)
    {
        pixmapItem->setPos(0, 0);
    }
}