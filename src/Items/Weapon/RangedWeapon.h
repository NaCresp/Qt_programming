#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"
#include <QTimer>

class RangedWeapon : public Weapon
{
    Q_OBJECT

public:
    explicit RangedWeapon(QObject *parent, const QString &pixmapPath, const QString &bulletPixmapPath, int damage, int fireRate, int maxAmmo);
    void attack() override;

    [[nodiscard]] int getCurrentAmmo() const;
    [[nodiscard]] int getMaxAmmo() const;

protected:
    int damage;
    int fireRate; // 毫秒
    QString bulletPixmapPath;
    QTimer* fireRateTimer;
    bool canFire;
    int maxAmmo;
    int currentAmmo;
};

#endif // RANGEDWEAPON_H