// src/Items/Item.cpp

#include "Item.h"

// 找到 Item 的建構函式並修改它
Item::Item(QObject *parent, const QString &pixmapPath)
    // 將 QGraphicsItem() 改為 QGraphicsItem(qobject_cast<QGraphicsItem*>(parent))
    : QObject(parent), QGraphicsItem(qobject_cast<QGraphicsItem*>(parent))
{
    if (!pixmapPath.isEmpty())
    {
        // 這裡的 'this' 是一個 QGraphicsItem*，所以 pixmapItem 的父級是自己
        pixmapItem = new QGraphicsPixmapItem(QPixmap(pixmapPath), this);
    }
}
