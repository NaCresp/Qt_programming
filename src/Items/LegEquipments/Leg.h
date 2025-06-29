//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_WELLWORNTROUSERS_H
#define QT_PROGRAMMING_2024_WELLWORNTROUSERS_H

#include "LegEquipment.h"
#include <QPixmap>

class WellWornTrousers : public LegEquipment
{
public:
    explicit WellWornTrousers(QObject *parent);

    void setSquatMode(bool enabled) override;

private:
    QPixmap normalPixmap;
    QPixmap squatPixmap;
};

#endif // QT_PROGRAMMING_2024_WELLWORNTROUSERS_H
