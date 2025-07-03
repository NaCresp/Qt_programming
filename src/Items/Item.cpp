// src/Items/Item.cpp

#include "Item.h"

// 找到 Item 的建构函式并修改它
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