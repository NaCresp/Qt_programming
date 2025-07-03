#include "Bandage.h"
#include "../Characters/Character.h"

Bandage::Bandage(QObject *parent) : Medicine(parent, ":/Items/Medicine/Belt/belt.png")
{
    // 设置在地面上时的大小
    setScale(0.1);
}

void Bandage::applyEffect(Character *character)
{
    character->heal(20);
}