#ifndef ADRENALINE_H
#define ADRENALINE_H

#include "Medicine.h"

class Adrenaline : public Medicine
{
public:
    explicit Adrenaline(QObject *parent = nullptr);
    void applyEffect(Character *character) override;
};

#endif // ADRENALINE_H