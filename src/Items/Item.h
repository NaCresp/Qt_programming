// src/Items/Item.h

#ifndef QT_PROGRAMMING_2024_ITEM_H
#define QT_PROGRAMMING_2024_ITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject> // 1. 包含 QObject 頭文件
#include <QPointF> // <-- 新增：包含 QPointF

// 2. 讓 Item 繼承 QObject 和 QGraphicsItem
//    注意：QObject 必須放在第一個
class Item : public QObject, public QGraphicsItem
{
    Q_OBJECT // 3. 加上 Q_OBJECT 巨集
    // 4. 聲明 QGraphicsItem 介面
    Q_INTERFACES(QGraphicsItem)

public:
    // 5. 修改建構函式，將 parent 從 QGraphicsItem* 改為 QObject*
    explicit Item(QObject *parent, const QString &pixmapPath);

    // ... 其他部分保持不變 ...
    [[nodiscard]] QRectF boundingRect() const override
    {
        if (pixmapItem != nullptr)
        {
            return this->pixmapItem->boundingRect();
        }
        return {};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override
    {
    }
    
    // --- 新增代码：为所有物品添加速度和重力方法 ---
    [[nodiscard]] const QPointF &getVelocity() const;
    void setVelocity(const QPointF &velocity);
    void applyGravity(qreal gravity);


protected:
    QGraphicsPixmapItem *pixmapItem{};
    QPointF velocity{}; // <-- 新增：所有物品都拥有速度向量
};

#endif // QT_PROGRAMMING_2024_ITEM_H