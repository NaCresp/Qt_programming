#ifndef QT_PROGRAMMING_2024_WEAPON_H
#define QT_PROGRAMMING_2024_WEAPON_H

#include "../Item.h"
#include <QPropertyAnimation>

class Weapon : public Item
{
    Q_OBJECT
public:
    explicit Weapon(QObject *parent, const QString &pixmapPath);
    virtual void attack() = 0;

protected:
    QPropertyAnimation *animation;
};

#endif // WEAPON_H
