#include "Knife.h"
#include <QPropertyAnimation>

Knife::Knife(QObject *parent) 
    : Weapon(parent, ":/Items/Weapon/Knife/knife.png")
{

    
    // 设置旋转中心
    setTransformOriginPoint(boundingRect().width() / 2, boundingRect().height());
    
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

    Weapon::mountToParent();

    setPos(-80, -200); 
}