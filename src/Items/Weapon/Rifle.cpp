#include "Rifle.h"

Rifle::Rifle(QObject *parent)
    : RangedWeapon(parent,
                   ":/Items/Weapon/Rifle/rifle.png",
                   ":/Items/Weapon/Bullet/bullet.png",
                   20, // 较低伤害
                   300) // 快射速
{
    // 步枪的特定属性（如果有）
}