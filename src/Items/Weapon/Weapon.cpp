#include "Weapon.h"

Weapon::Weapon(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::OutCubic);
}

void Weapon::mountToParent()
{
    Mountable::mountToParent();
    setPos(-20, -105); 
    setScale(0.4); // 装备在身上时的大小
    setVisible(true);
    setZValue(1); // 确保在角色图层之上
}

void Weapon::unmount()
{
    Mountable::unmount();
    setScale(0.1); // <-- 增大掉落在地上时的大小，更容易看见
    setVisible(true);
    setZValue(1); // 确保在地图之上
}