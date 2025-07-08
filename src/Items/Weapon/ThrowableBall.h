#ifndef THROWABLEBALL_H
#define THROWABLEBALL_H

#include "../Item.h"
#include <QPointF>

// 向前声明 Character 类，避免循环引用
class Character;

class ThrowableBall : public Item
{
    Q_OBJECT

public:
    // 在构造函数中接收所有者、初始速度和伤害
    explicit ThrowableBall(QObject *parent, Character* owner, const QPointF& initialVelocity, int damage);

    int getDamage() const;
    Character* getOwner() const; // 获取所有者

private:
    Character* owner; // 存储子弹的拥有者
    int damage;
};

#endif // THROWABLEBALL_H