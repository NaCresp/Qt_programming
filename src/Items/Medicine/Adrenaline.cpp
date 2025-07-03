#include "Adrenaline.h"
#include "../Characters/Character.h"

Adrenaline::Adrenaline(QObject *parent) : Medicine(parent, ":/Items/Medicine/Adrenaline/adrenaline.png")
{
    // 设置在地面上时的大小
    setScale(0.1);
}

void Adrenaline::applyEffect(Character *character)
{
    character->applyAdrenalineBuff();
}