#ifndef QT_PROGRAMMING_2024_LIGHTARMOR_H
#define QT_PROGRAMMING_2024_LIGHTARMOR_H

#include "Armor.h"

class LightArmor : public Armor
{
public:
    explicit LightArmor(QObject *parent = nullptr);

    // --- 新增：重写挂载和卸载方法 ---
    void mountToParent() override;
    void unmount() override;
};

#endif // QT_PROGRAMMING_2024_LIGHTARMOR_H