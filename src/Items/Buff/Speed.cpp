//
// Created by gerw on 8/22/24.
//

#include "Speed.h"

Speed::Speed(QObject *parent) : Item(parent, ":/Items/Buff/Speed/speed.png")
{
    // 构造函数可以为空
}

void Speed::mountToParent()
{
    Mountable::mountToParent();
    setScale(0.3); // 根据你的图标大小调整
    setPos(0, -220); // 调整图标在角色身上的位置
    setZValue(5);
    // --- 修正结束 ---
}