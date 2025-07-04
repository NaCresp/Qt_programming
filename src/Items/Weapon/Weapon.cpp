#include "Weapon.h"

Weapon::Weapon(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath)
{
    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::OutCubic);
}

void Weapon::mountToParent()
{
    Mountable::mountToParent();
    setPos(-20, -105); 
    setScale(0.4); // 装备在身上时的大小
    setVisible(true);
    setZValue(1); // 确保在角色图层之上
    
    // 装备时恢复贴图的相对位置
    if (pixmapItem) {
        pixmapItem->setPos(0, 0);
    }
}

void Weapon::unmount()
{
    Mountable::unmount();
    setScale(0.1); // <-- 增大掉落在地上时的大小，更容易看见
    setVisible(true);
    setZValue(1); // 确保在地图之上

    // --- 新增代码：校正掉落时的贴图位置 ---
    // 缩放后，将贴图向上移动，使其底部与物品的逻辑位置对齐
    if (pixmapItem) {
        pixmapItem->setPos(0, -100);
    }
}