//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_CHARACTER_H
#define QT_PROGRAMMING_2024_CHARACTER_H

#include <QGraphicsEllipseItem>
#include "../HeadEquipments/HeadEquipment.h"
#include "../Armors/Armor.h"
#include "../LegEquipments/LegEquipment.h"
#include "../Weapon/Weapon.h"
#include "../Buff/Speed.h"
#include "../Buff/Health.h" // 新增
#include "../Armors/HeavyArmor.h" // 新增重甲头文件

class Character : public Item
{
    Q_OBJECT

public:
    explicit Character(QObject *parent);

    QRectF boundingRect() const override;

    [[nodiscard]] bool isLeftDown() const;
    void setLeftDown(bool leftDown);
    [[nodiscard]] bool isRightDown() const;
    void setRightDown(bool rightDown);
    [[nodiscard]] bool isPickDown() const;
    void setPickDown(bool pickDown);
    // --- 已移除 ---: getVelocity 和 setVelocity 方法 (已继承自 Item)
    [[nodiscard]] bool isPicking() const;
    void setAttackDown(bool attackDown);
    [[nodiscard]] bool isAttackDown() const;

    void setSquatDown(bool squatDown);
    [[nodiscard]] bool isSquatting() const;

    void attack();

    void processInput();
    void jump();
    // --- 已移除 ---: applyGravity 方法 (已继承自 Item)

    void setOnGround(bool onGround);
    [[nodiscard]] bool isOnGround() const;

    void takeDamage(int amount, Weapon* sourceWeapon); // 修改：增加伤害来源
    void heal(int amount);
    [[nodiscard]] int getCurrentHp() const;
    [[nodiscard]] int getMaxHp() const;
    [[nodiscard]] Weapon* getWeapon() const;
    [[nodiscard]] Armor* getArmor() const; // 新增

    // --- 新增代码：添加Buff图标的getter ---
    [[nodiscard]] Speed* getSpeedBuffIcon() const;
    [[nodiscard]] Health* getHealthBuffIcon() const; // 新增
    // --- 新增代码结束 ---

    Armor *pickupArmor(Armor *newArmor);
    void pickupWeapon(Weapon *newWeapon);

    void applySpeedBuff();
    void removeSpeedBuff();
    [[nodiscard]] bool hasSpeedBuff() const;

    void applyAdrenalineBuff(); // 新增
    void checkWeaponAmmo();


signals:
    void healthChanged(int amount, const QPointF &position);
    void shieldChanged(); // 新增

private slots:
    void onShieldBroken(); // 新增

protected:
    HeadEquipment *headEquipment{};
    LegEquipment *legEquipment{};
    Armor *armor{};
    Weapon *weapon{};
    Speed *speedBuffIcon{};
    Health *healthBuffIcon{}; // 新增
    // --- 已移除 ---: velocity 向量 (已继承自 Item)

private:
    bool leftDown{}, rightDown{}, pickDown{};
    bool lastPickDown{};
    bool picking{};
    bool onGround{true};

    bool attackDown{};
    bool lastAttackDown{};
    bool attacking{};

    bool squatDown{};
    bool isSquattingState{};
    void squat();
    void standUp();

    int maxHp;
    int currentHp;

    bool isSpeedBuffed{false};
    qreal speedMultiplier{1.0};

    // --- 新增肾上腺素相关 ---
    QTimer *adrenalineTimer{};
    int adrenalineTicks{0};
    void handleAdrenalineTick();
    // --- 新增结束 ---
};

#endif // QT_PROGRAMMING_2024_CHARACTER_H