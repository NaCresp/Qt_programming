#ifndef QT_PROGRAMMING_2024_SPEED_H
#define QT_PROGRAMMING_2024_SPEED_H

#include "../Item.h"
#include "../Mountable.h"

class Speed : public Item, public Mountable
{
public:
    explicit Speed(QObject *parent = nullptr);
    void mountToParent() override;
};

#endif // QT_PROGRAMMING_2024_SPEED_H