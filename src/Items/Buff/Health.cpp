#include "Health.h"

Health::Health(QObject *parent) : Item(parent, ":/Items/Buff/Health/health.png")
{
}

void Health::mountToParent()
{
    Mountable::mountToParent();
    setScale(0.3);
    // 调整图标在角色头上的位置，这里假设它在速度buff旁边
    setPos(40, -220);
    setZValue(5);
}