#include "Rifle.h"

Rifle::Rifle(QObject *parent)
    : RangedWeapon(parent,
                   ":/Items/Weapon/Rifle/rifle.png",
                   ":/Items/Weapon/Bullet/bullet.png",
                   20,    // 伤害
                   300,   // 射速
                   20)    // 弹药量
{
}