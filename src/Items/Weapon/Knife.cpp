#include "Knife.h"
#include <QPropertyAnimation>

Knife::Knife(QObject *parent) 
    : Weapon(parent, ":/Items/Weapon/Knife/knife.png")
{
    // 初始化动画组
    attackSequence = new QSequentialAnimationGroup(this);
    
    auto forwardAnimation = new QPropertyAnimation(this, "rotation");
    forwardAnimation->setDuration(150);
    forwardAnimation->setStartValue(0);
    forwardAnimation->setEndValue(-90);
    forwardAnimation->setEasingCurve(QEasingCurve::OutQuad);

    auto backwardAnimation = new QPropertyAnimation(this, "rotation");
    backwardAnimation->setDuration(200);
    backwardAnimation->setStartValue(-90);
    backwardAnimation->setEndValue(0);
    backwardAnimation->setEasingCurve(QEasingCurve::InQuad);

    attackSequence->addAnimation(forwardAnimation);
    attackSequence->addAnimation(backwardAnimation);
}

void Knife::attack()
{
    if (attackSequence->state() == QAbstractAnimation::Running) {
        return;
    }
    hasDealtDamage = false; 
    attackSequence->start();
}


void Knife::mountToParent()
{
    // 首先调用基类的通用逻辑
    Weapon::mountToParent();

    // 然后应用小刀专属的设置
    setPos(-80, -200); 
    // 仅在装备时才设置特殊的旋转中心，用于攻击动画
    setTransformOriginPoint(boundingRect().width() / 2, boundingRect().height());
}

void Knife::unmount()
{
    // 首先调用基类的通用卸载逻辑（这会把大小设置为0.25）
    Weapon::unmount();

    // 然后，将旋转中心重置回默认值(0,0)
    setTransformOriginPoint(0, 0);

    // --- 新增代码：为小刀设置一个更大的专属尺寸 ---
    setScale(0.15); // <-- 这会覆盖基类设置的大小，只对小刀生效
}