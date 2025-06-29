#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::OutCubic);
}