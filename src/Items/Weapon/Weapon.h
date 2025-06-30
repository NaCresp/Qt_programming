#ifndef QT_PROGRAMMING_2024_WEAPON_H
#define QT_PROGRAMMING_2024_WEAPON_H

#include "../Item.h"
#include <QPropertyAnimation>

class Weapon : public Item
{
    Q_OBJECT // <--- 必须添加此行

public:
    explicit Weapon(QObject *parent, const QString &pixmapPath);
    virtual void attack() = 0;

    // --- 新增代码 ---
    // 用于标记单次攻击是否已造成伤害，防止重复扣血
    bool hasDealtDamage{false};
    // --- 新增代码结束 ---

protected:
    QPropertyAnimation *animation;
};

#endif // WEAPON_H
