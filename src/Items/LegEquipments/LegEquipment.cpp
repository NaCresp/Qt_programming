//
// Created by gerw on 8/20/24.
//

#include "LegEquipment.h"

LegEquipment::LegEquipment(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
}

void LegEquipment::mountToParent()
{
    Mountable::mountToParent();
    setScale(0.8);
    setPos(-10, -40);
}
