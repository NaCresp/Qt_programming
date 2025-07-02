#ifndef KNIFE_H
#define KNIFE_H

#include "Weapon.h"
#include <QSequentialAnimationGroup> 

class Knife : public Weapon
{
    Q_OBJECT

public:
    explicit Knife(QObject *parent = nullptr);
    void attack() override;

private:
    // 我们将用一个动画组来管理整个攻击动作
    QSequentialAnimationGroup *attackSequence; 
};

#endif // KNIFE_H