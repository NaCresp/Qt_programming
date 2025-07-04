#include "RangedWeapon.h"
#include "Bullet.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>

RangedWeapon::RangedWeapon(QObject *parent, const QString &pixmapPath, const QString &bulletPixmapPath, int damage, int fireRate)
    : Weapon(parent, pixmapPath), damage(damage), fireRate(fireRate), bulletPixmapPath(bulletPixmapPath), canFire(true)
{
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer, &QTimer::timeout, [&]() {
        canFire = true;
        fireRateTimer->stop();
    });
}

void RangedWeapon::attack()
{
    if (!canFire)
    {
        return; // 仍在冷却中
    }

    auto ownerCharacter = dynamic_cast<Character*>(parentItem());
    if (!ownerCharacter)
    {
        return; // 如果武器没有装备在角色上，则无法攻击
    }

    // 根据角色视觉上的翻转来决定正确的子弹方向
    QPointF direction;
    // 核心修正：将方向判断的逻辑反转
    if (parentItem()->transform().m11() < 0) {
        // 如果角色在X轴上被翻转了（看起来朝左），那么子弹应该朝右
        direction = QPointF(1, 0);
    } else {
        // 如果角色没有被翻转（看起来朝右），那么子弹应该朝左
        direction = QPointF(-1, 0);
    }

    // 创建子弹
    auto bullet = new Bullet(nullptr, ownerCharacter, bulletPixmapPath, direction, damage);
    // 将子弹的初始位置设置在武器前方
    bullet->setPos(this->sceneBoundingRect().center() + direction * 30);

    if (scene())
    {
        scene()->addItem(bullet);
    }

    canFire = false;
    fireRateTimer->start(fireRate);
}