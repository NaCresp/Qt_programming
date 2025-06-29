#include "Fist.h"
#include <QTimer>

Fist::Fist(QObject *parent) : Weapon(parent, ":/Items/Weapon/Fist/fist.png")
{
    setPos(20, -90);
    setScale(0.2);
    setVisible(false);
}

void Fist::attack()
{
    if (isVisible())
    {
        return;
    }
    setVisible(true);
    QPointF startPos = pos();
    QPointF endPos = startPos + QPointF(50, 0);

    animation->setStartValue(startPos);
    animation->setEndValue(endPos);
    animation->start();

    QTimer::singleShot(200, this, [this]()
                       {
        setVisible(false);
        setPos(20, -90); });
}
