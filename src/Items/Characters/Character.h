//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_CHARACTER_H
#define QT_PROGRAMMING_2024_CHARACTER_H

#include <QGraphicsEllipseItem>
#include "../HeadEquipments/HeadEquipment.h"
#include "../Armors/Armor.h"
#include "../LegEquipments/LegEquipment.h"

class Character : public Item {
public:
    explicit Character(QGraphicsItem *parent);

    [[nodiscard]] bool isLeftDown() const;
    void setLeftDown(bool leftDown);
    [[nodiscard]] bool isRightDown() const;
    void setRightDown(bool rightDown);
    [[nodiscard]] bool isPickDown() const;
    void setPickDown(bool pickDown);
    [[nodiscard]] const QPointF &getVelocity() const;
    void setVelocity(const QPointF &velocity);
    [[nodiscard]] bool isPicking() const;

    void processInput();
    void jump();
    void applyGravity(qreal gravity); // 简化：只应用重力，不检测碰撞

    // 新增方法，用于从外部设置和查询地面状态
    void setOnGround(bool onGround);
    [[nodiscard]] bool isOnGround() const;

    Armor* pickupArmor(Armor* newArmor);

protected:
    HeadEquipment *headEquipment{};
    LegEquipment *legEquipment{};
    Armor *armor{};
    QPointF velocity{};
    //    QGraphicsEllipseItem *ellipseItem; // for debugging
private:
    bool leftDown{}, rightDown{}, pickDown{};
    bool lastPickDown{};
    bool picking{};
    bool onGround{true};
};


#endif //QT_PROGRAMMING_2024_CHARACTER_H
