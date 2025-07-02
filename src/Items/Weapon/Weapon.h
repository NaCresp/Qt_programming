#ifndef QT_PROGRAMMING_2024_WEAPON_H
#define QT_PROGRAMMING_2024_WEAPON_H

#include "../Item.h"
#include "../Mountable.h"
#include <QPropertyAnimation>

class Weapon : public Item, public Mountable
{
    Q_OBJECT
    // --- **核心修正：添加 Q_PROPERTY** ---
    // 这行代码告诉 Qt 的属性系统：
    // 1. 我有一个名为 'rotation' 的属性。
    // 2. 读取这个属性的值，请调用 QGraphicsItem::rotation() 函数。
    // 3. 设置这个属性的值，请调用 QGraphicsItem::setRotation() 函数。
    // 这样，QPropertyAnimation 就能找到并正确地动画化这个属性了。
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)

public:
    explicit Weapon(QObject *parent, const QString &pixmapPath);
    virtual void attack() = 0;

    void mountToParent() override;
    void unmount() override;

    bool hasDealtDamage{false};

protected:
    QPropertyAnimation *animation;
};

#endif // WEAPON_H