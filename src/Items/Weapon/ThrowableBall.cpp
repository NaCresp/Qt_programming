#include "ThrowableBall.h"
#include "../Characters/Character.h" // 包含完整的头文件

// 构造函数
ThrowableBall::ThrowableBall(QObject *parent, Character* owner, const QPointF& initialVelocity, int damage)
    : Item(parent, ":/Items/Weapon/Ball/ball.png"), owner(owner), damage(damage)
{
    // 设置初始速度
    setVelocity(initialVelocity);
    setScale(0.15); // 设置球的大小
}

int ThrowableBall::getDamage() const
{
    return damage;
}

// 实现 getOwner 函数
Character* ThrowableBall::getOwner() const
{
    return owner;
}