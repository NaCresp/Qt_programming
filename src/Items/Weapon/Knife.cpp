#include "Knife.h"
#include <QPropertyAnimation>

Knife::Knife(QObject *parent) 
    : Weapon(parent, ":/Items/Weapon/Knife/knife.png")
{
    // 1. 初始化顺序动画组
    attackSequence = new QSequentialAnimationGroup(this);
    
    // 2. 创建“向前挥刀”的动画
    auto forwardAnimation = new QPropertyAnimation(this, "rotation");
    forwardAnimation->setDuration(150); // 挥刀速度
    forwardAnimation->setStartValue(0);
    forwardAnimation->setEndValue(-90);  // 旋转90度
    forwardAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // 3. 创建“向后收刀”的动画 (只是一个方向相反的动画)
    auto backwardAnimation = new QPropertyAnimation(this, "rotation");
    backwardAnimation->setDuration(200); // 收刀可以慢一点
    backwardAnimation->setStartValue(-90);
    backwardAnimation->setEndValue(0);
    backwardAnimation->setEasingCurve(QEasingCurve::InQuad);

    // 4. 将两个动画按顺序添加到播放列表中
    attackSequence->addAnimation(forwardAnimation);
    attackSequence->addAnimation(backwardAnimation);
}

void Knife::attack()
{
    // 如果整个攻击序列（挥刀+收刀）正在播放，则不允许再次攻击
    if (attackSequence->state() == QAbstractAnimation::Running) {
        return;
    }

    // 每次攻击前，重置伤害标记
    hasDealtDamage = false; 

    // 播放整个动画序列
    attackSequence->start();
}