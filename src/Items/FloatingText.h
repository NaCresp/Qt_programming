#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <QGraphicsTextItem>
#include <QPainter>

// 此类现在只是一个简单的 QGraphicsTextItem，没有动画和定时器
class FloatingText : public QGraphicsTextItem
{
public:
    explicit FloatingText(const QString &text, const QColor &color, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // FLOATINGTEXT_H