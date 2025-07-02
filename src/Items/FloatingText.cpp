#include "FloatingText.h"
#include <QFont>

// 构造函数现在非常简单，只设置外观
FloatingText::FloatingText(const QString &text, const QColor &color, QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setPlainText(text);
    setDefaultTextColor(color);
    setFont(QFont("Arial", 16, QFont::Bold));
    setZValue(100); // 确保在最上层
}

void FloatingText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsTextItem::paint(painter, option, widget);
}