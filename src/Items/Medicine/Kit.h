#ifndef KIT_H
#define KIT_H

#include "Medicine.h"

class Kit : public Medicine
{
public:
    explicit Kit(QObject *parent = nullptr);
    void applyEffect(Character *character) override;
};

#endif // KIT_H