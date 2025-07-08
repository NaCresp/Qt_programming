#include "Ball.h"
#include "ThrowableBall.h"
#include "../Characters/Character.h"
#include <QGraphicsScene>

Ball::Ball(QObject *parent)
    : Weapon(parent, ":/Items/Weapon/Ball/ball.png"), canFire(true), maxAmmo(5), currentAmmo(5)
{
    // 设置射速计时器
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer, &QTimer::timeout, [&]() {
        canFire = true;
        fireRateTimer->stop();
    });
}

void Ball::attack()
{
    // 如果不能开火或没有弹药，则返回
    if (!canFire || currentAmmo <= 0)
    {
        return;
    }

    // 获取武器的所有者（即角色）
    auto ownerCharacter = dynamic_cast<Character*>(parentItem());
    if (!ownerCharacter)
    {
        return;
    }

    // --- **修正开始** ---
    // 根据角色的朝向决定初始速度
    qreal initialSpeedX = 10.0;
    qreal initialSpeedY = -12.0; // 向上-12

    // parentItem()->transform().m11() > 0 (即为 1) 表示角色未翻转，朝向左边
    // parentItem()->transform().m11() < 0 (即为 -1) 表示角色水平翻转，朝向右边
    if (ownerCharacter->transform().m11() > 0) { // 朝向左
        initialSpeedX = -initialSpeedX; // 水平速度应为负
    }
    // 如果朝向右，initialSpeedX 保持为正，这是正确的
    // --- **修正结束** ---

    QPointF initialVelocity(initialSpeedX, initialSpeedY);

    // 创建一个可投掷的球
    auto throwableBall = new ThrowableBall(nullptr, ownerCharacter, initialVelocity, 10); // 伤害为10
    // 设置球的初始位置在角色中心
    throwableBall->setPos(this->sceneBoundingRect().center());

    // 将球添加到场景中
    if (scene())
    {
        scene()->addItem(throwableBall);
    }

    currentAmmo--; // 减少弹药

    // 开始冷却
    canFire = false;
    fireRateTimer->start(1000); // 1秒冷却时间
}

int Ball::getCurrentAmmo() const
{
    return currentAmmo;
}

int Ball::getMaxAmmo() const
{
    return maxAmmo;
}