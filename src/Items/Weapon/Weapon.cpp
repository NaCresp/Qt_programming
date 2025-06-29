#include "Weapon.h"

Weapon::Weapon(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::OutCubic);
}
