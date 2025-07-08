#ifndef QT_PROGRAMMING_2024_HEAVYARMOR_H
#define QT_PROGRAMMING_2024_HEAVYARMOR_H

#include "Armor.h"

class HeavyArmor : public Armor
{
    Q_OBJECT
public:
    explicit HeavyArmor(QObject *parent = nullptr);

    void takeShieldDamage(int amount);
    [[nodiscard]] int getShield() const;
    [[nodiscard]] int getMaxShield() const;

    // --- 新增：重写挂载和卸载方法 ---
    void mountToParent() override;
    void unmount() override;

signals:
    void shieldChanged(int newShield);
    void shieldBroken();

private:
    int maxShield;
    int currentShield;
};

#endif // QT_PROGRAMMING_2024_HEAVYARMOR_H