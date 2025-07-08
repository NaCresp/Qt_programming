#include "HeavyArmor.h"

HeavyArmor::HeavyArmor(QObject *parent)
    : Armor(parent, ":/Items/Armors/Heavy/heavy.png"), maxShield(100), currentShield(100)
{
}

void HeavyArmor::mountToParent()
{
    // 首先调用基类的通用逻辑
    Armor::mountToParent();
    // 然后在这里设置重甲专属的穿戴位置
    // 例如，重甲看起来更厚重，可以稍微向下调整
    setPos(0, -90);
    setScale(0.62); // 大小也可以调整
}

void HeavyArmor::unmount()
{
    // 首先调用基类的通用逻辑
    Armor::unmount();
    // 然后在这里设置重甲专属的掉落位置和外观
    // 例如，让它在地上时也显得大一些
    setScale(0.28);
    // 同样，调整贴图位置
    if (pixmapItem != nullptr)
    {
        pixmapItem->setPos(0, 0);
    }
}


void HeavyArmor::takeShieldDamage(int amount)
{
    currentShield -= amount;
    if (currentShield <= 0)
    {
        currentShield = 0;
        emit shieldBroken();
    }
    emit shieldChanged(currentShield);
}

int HeavyArmor::getShield() const
{
    return currentShield;
}

int HeavyArmor::getMaxShield() const
{
    return maxShield;
}