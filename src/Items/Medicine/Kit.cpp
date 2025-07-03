#include "Kit.h"
#include "../Characters/Character.h"

Kit::Kit(QObject *parent) : Medicine(parent, ":/Items/Medicine/Kit/kit.png")
{
    // 设置在地面上时的大小
    setScale(0.1);
}

void Kit::applyEffect(Character *character)
{
    character->heal(character->getMaxHp());
}