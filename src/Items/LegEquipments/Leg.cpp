//
// Created by gerw on 8/20/24.
//

#include "Leg.h"

WellWornTrousers::WellWornTrousers(QObject *parent)
    : LegEquipment(parent, ":/Items/LegEquipments/Leg/leg.png"),
    normalPixmap(QPixmap(":/Items/LegEquipments/Leg/leg.png")),
    squatPixmap(QPixmap(":/Items/LegEquipments/SquatLeg/squatLeg.png"))
{
}

void WellWornTrousers::setSquatMode(bool enabled)
{
    if (pixmapItem)
    {
        if (enabled)
        {
            pixmapItem->setPixmap(squatPixmap);
        }
        else
        {
            pixmapItem->setPixmap(normalPixmap);
        }
    }
}
