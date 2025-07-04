#ifndef BULLET_H
#define BULLET_H

#include "../Item.h"
#include <QPointF>

// 向前声明 Character 类，避免循环引用
class Character;

class Bullet : public Item
{
    Q_OBJECT

public:
    // 在构造函数中接收一个 Character* 作为 owner
    explicit Bullet(QObject *parent, Character* owner, const QString& pixmapPath, const QPointF& direction, int damage);
    void updatePosition();
    int getDamage() const;
    Character* getOwner() const; // 新增一个 getter 来获取 owner

private:
    Character* owner; // 新增一个成员变量来存储子弹的拥有者
    QPointF velocity;
    int damage;
};

#endif // BULLET_H