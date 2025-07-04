#include "RangedWeapon.h"
#include "Bullet.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>

RangedWeapon::RangedWeapon(QObject *parent, const QString &pixmapPath, const QString &bulletPixmapPath, int damage, int fireRate, int maxAmmo)
    : Weapon(parent, pixmapPath), damage(damage), fireRate(fireRate), bulletPixmapPath(bulletPixmapPath), canFire(true), maxAmmo(maxAmmo), currentAmmo(maxAmmo)
{
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer, &QTimer::timeout, [&]() {
        canFire = true;
        fireRateTimer->stop();
    });
}

void RangedWeapon::attack()
{
    if (!canFire || currentAmmo <= 0)
    {
        return; 
    }

    auto ownerCharacter = dynamic_cast<Character*>(parentItem());
    if (!ownerCharacter)
    {
        return; 
    }

    QPointF direction;
    if (parentItem()->transform().m11() < 0) {
        direction = QPointF(1, 0);
    } else {
        direction = QPointF(-1, 0);
    }

    auto bullet = new Bullet(nullptr, ownerCharacter, bulletPixmapPath, direction, damage);
    bullet->setPos(this->sceneBoundingRect().center() + direction * 30);

    if (scene())
    {
        scene()->addItem(bullet);
    }
    
    currentAmmo--; // 减少弹药

    canFire = false;
    fireRateTimer->start(fireRate);
}

int RangedWeapon::getCurrentAmmo() const
{
    return currentAmmo;
}

int RangedWeapon::getMaxAmmo() const
{
    return maxAmmo;
}