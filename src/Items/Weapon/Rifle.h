#ifndef RIFLE_H
#define RIFLE_H

#include "RangedWeapon.h"

class Rifle : public RangedWeapon
{
public:
    explicit Rifle(QObject *parent = nullptr);
};

#endif // RIFLE_H