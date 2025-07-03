#ifndef MEDICINE_H
#define MEDICINE_H

#include "../Item.h"
#include "../Mountable.h"

class Character; // 前向声明

class Medicine : public Item, public Mountable
{
public:
    explicit Medicine(QObject *parent, const QString &pixmapPath);
    void mountToParent() override;
    // 定义一个纯虚函数，让所有医疗品都实现自己的效果
    virtual void applyEffect(Character *character) = 0;
};

#endif // MEDICINE_H