//
// Created by gerw on 8/20/24.
//

#include "Armor.h"

Armor::Armor(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
}

void Armor::mountToParent()
{
    Mountable::mountToParent();
    setScale(0.6);
    setPos(2, -108);
    if (pixmapItem != nullptr)
    {
        pixmapItem->setPos(0, 0);
    }
}

void Armor::unmount()
{
    Mountable::unmount();
    setScale(0.2);
    if (pixmapItem != nullptr)
    {
        pixmapItem->setPos(0, -120);
    }
}
