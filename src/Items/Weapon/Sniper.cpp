#include "Sniper.h"

Sniper::Sniper(QObject *parent)
    : RangedWeapon(parent,
                   ":/Items/Weapon/Sniper/sniper.png",
                   ":/Items/Weapon/Bullet/bullet.png",
                   50,   // 伤害
                   1500, // 射速
                   5)    // 弹药量
{
}