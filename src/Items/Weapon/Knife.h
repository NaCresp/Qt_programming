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
    
    // --- 修改：同时重写 mountToParent 和 unmount ---
    void mountToParent() override;
    void unmount() override; // <-- 新增此行

private:
    // 我们将用一个动画组来管理整个攻击动作
    QSequentialAnimationGroup *attackSequence; 
};

#endif // KNIFE_H