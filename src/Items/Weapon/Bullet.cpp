#include "Bullet.h"
#include "../Characters/Character.h" // 包含完整的头文件

// 更新构造函数以保存 owner
Bullet::Bullet(QObject *parent, Character* owner, const QString& pixmapPath, const QPointF& direction, int damage)
    : Item(parent, pixmapPath), owner(owner), damage(damage)
{
    qreal speed = 15.0;
    velocity = direction * speed;
    setScale(0.2);
}

void Bullet::updatePosition()
{
    setPos(pos() + velocity);
}

int Bullet::getDamage() const
{
    return damage;
}

// 实现 getOwner 函数
Character* Bullet::getOwner() const
{
    return owner;
}