#ifndef BANDAGE_H
#define BANDAGE_H

#include "Medicine.h"

class Bandage : public Medicine
{
public:
    explicit Bandage(QObject *parent = nullptr);
    void applyEffect(Character *character) override;
};

#endif // BANDAGE_H