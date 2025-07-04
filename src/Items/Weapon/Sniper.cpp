#include "Sniper.h"

Sniper::Sniper(QObject *parent)
    : RangedWeapon(parent,
                   ":/Items/Weapon/Sniper/sniper.png",
                   ":/Items/Weapon/Bullet/bullet.png",
                   50,  // 高伤害
                   1500) // 慢射速
{
    // 狙击枪的特定属性（如果有）
}