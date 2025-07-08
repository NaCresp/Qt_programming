#ifndef BALL_H
#define BALL_H

#include "Weapon.h"
#include <QTimer>

class Ball : public Weapon
{
    Q_OBJECT

public:
    explicit Ball(QObject *parent = nullptr);
    void attack() override;

    [[nodiscard]] int getCurrentAmmo() const;
    [[nodiscard]] int getMaxAmmo() const;

private:
    QTimer* fireRateTimer;
    bool canFire;
    int maxAmmo;
    int currentAmmo;
};

#endif // BALL_H