#ifndef QT_PROGRAMMING_2024_FIST_H
#define QT_PROGRAMMING_2024_FIST_H

#include "Weapon.h"
#include <QTimer>      // 包含 QTimer
#include <QElapsedTimer> // 包含 QElapsedTimer

class Fist : public Weapon
{
    Q_OBJECT // 保留 Q_OBJECT 宏是一个好习惯，但此方案不依赖它

public:
    explicit Fist(QObject *parent);
    void attack() override;

private slots:
    void updateAnimation(); // 用于更新动画每一帧的槽函数

private:
    QTimer *animationTimer;          // 用来驱动动画帧更新的计时器
    QElapsedTimer *elapsedTimer;     // 用来精确计算动画已进行时间的计时器
    QPointF animationStartPos;       // 动画的起始位置
    QPointF animationEndPos;         // 动画的目标位置
    bool isAttacking;                // 一个简单的状态锁，防止重复攻击
    static const int ANIMATION_DURATION = 100; // 动画时长 (毫秒)
};

#endif // FIST_H
