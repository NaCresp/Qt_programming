#include "Medicine.h"

Medicine::Medicine(QObject *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {}

void Medicine::mountToParent()
{
    // 医疗品在拾取后即被消耗，所以“装备”时就将它们隐藏
    Mountable::mountToParent();
    setVisible(false);
}