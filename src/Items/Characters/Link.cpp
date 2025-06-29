//
// Created by gerw on 8/20/24.
//

#include <memory>
#include "Link.h"
#include "../HeadEquipments/Head.h"
#include "../Armors/body.h"
#include "../LegEquipments/Leg.h"

Link::Link(QGraphicsItem *parent) : Character(parent) {
    headEquipment = new CapOfTheHero(this);
    legEquipment = new WellWornTrousers(this);
    armor = new OldShirt(this);
    headEquipment->mountToParent();
    legEquipment->mountToParent();
    armor->mountToParent();
}
