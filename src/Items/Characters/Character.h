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

class Character : public Item
{
    Q_OBJECT // <--- 确保这里有 Q_OBJECT 宏

public:
    explicit Character(QObject *parent);

    [[nodiscard]] bool isLeftDown() const;
    void setLeftDown(bool leftDown);
    [[nodiscard]] bool isRightDown() const;
    void setRightDown(bool rightDown);
    [[nodiscard]] bool isPickDown() const;
    void setPickDown(bool pickDown);
    [[nodiscard]] const QPointF &getVelocity() const;
    void setVelocity(const QPointF &velocity);
    [[nodiscard]] bool isPicking() const;
    void setAttackDown(bool attackDown);
    [[nodiscard]] bool isAttackDown() const;

    void setSquatDown(bool squatDown);
    [[nodiscard]] bool isSquatting() const;

    void attack();

    void processInput();
    void jump();
    void applyGravity(qreal gravity); // 简化：只应用重力，不检测碰撞

    void setOnGround(bool onGround);
    [[nodiscard]] bool isOnGround() const;

    // --- 修改/新增代码：血量系统 ---
    void takeDamage(int amount);
    void heal(int amount); // 新增治疗方法
    [[nodiscard]] int getCurrentHp() const;
    [[nodiscard]] int getMaxHp() const;
    [[nodiscard]] Weapon* getWeapon() const; // 新增一个获取武器的公有方法，用于碰撞检测

    Armor *pickupArmor(Armor *newArmor);
    Weapon *pickupWeapon(Weapon *newWeapon); // <-- 新增武器拾取函数

signals:
    // 新增信号，当血量变化时发射。参数：变化的数值，发生的位置
    void healthChanged(int amount, const QPointF &position);

protected:
    HeadEquipment *headEquipment{};
    LegEquipment *legEquipment{};
    Armor *armor{};
    Weapon *weapon{};
    QPointF velocity{};
    //    QGraphicsEllipseItem *ellipseItem; // for debugging
private:
    bool leftDown{}, rightDown{}, pickDown{};
    bool lastPickDown{};
    bool picking{};
    bool onGround{true};

    bool attackDown{};
    bool lastAttackDown{};
    bool attacking{};

    bool squatDown{};
    bool isSquattingState{}; // 使用新名称以避免与函数名冲突
    void squat();
    void standUp();

    // --- 新增代码：血量系统 ---
    int maxHp;
    int currentHp;
};

#endif // QT_PROGRAMMING_2024_CHARACTER_H