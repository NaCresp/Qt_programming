//
// Created by gerw on 8/20/24.
//

#include "HeadEquipment.h"

HeadEquipment::HeadEquipment(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
}

void HeadEquipment::mountToParent()
{
    Mountable::mountToParent();
    setScale(0.6);
    setPos(-55, -180);
}
