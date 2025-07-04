// src/Items/Item.cpp

#include "Item.h"

// 找到 Item 的建構函式并修改它
Item::Item(QObject *parent, const QString &pixmapPath)
    // 将 QGraphicsItem() 改为 QGraphicsItem(qobject_cast<QGraphicsItem*>(parent))
    : QObject(parent), QGraphicsItem(qobject_cast<QGraphicsItem*>(parent))
{
    if (!pixmapPath.isEmpty())
    {
        // 这里的 'this' 是一个 QGraphicsItem*，所以 pixmapItem 的父级是自己
        pixmapItem = new QGraphicsPixmapItem(QPixmap(pixmapPath), this);
    }
}

// --- 新增代码：实现速度和重力方法 ---
const QPointF &Item::getVelocity() const
{
    return velocity;
}

void Item::setVelocity(const QPointF &newVelocity)
{
    this->velocity = newVelocity;
}

void Item::applyGravity(qreal gravity)
{
    this->velocity.setY(this->velocity.y() + gravity);
}