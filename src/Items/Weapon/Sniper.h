#ifndef SNIPER_H
#define SNIPER_H

#include "RangedWeapon.h"

class Sniper : public RangedWeapon
{
public:
    explicit Sniper(QObject *parent = nullptr);
};

#endif // SNIPER_H