#ifndef QT_PROGRAMMING_2024_HEALTH_H
#define QT_PROGRAMMING_2024_HEALTH_H

#include "../Item.h"
#include "../Mountable.h"

class Health : public Item, public Mountable
{
public:
    explicit Health(QObject *parent = nullptr);
    void mountToParent() override;
};

#endif // QT_PROGRAMMING_2024_HEALTH_H