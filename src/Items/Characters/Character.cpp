//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include "Character.h"

const qreal JUMP_STRENGTH = -15.0; // 定义跳跃力度

Character::Character(QGraphicsItem *parent) : Item(parent, "") {
    //    ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
    //    // Optionally, set some properties of the ellipse
    //    ellipseItem->setBrush(Qt::green);          // Fill color
    //    ellipseItem->setZValue(1);
}

// ... (isLeftDown, setLeftDown, isRightDown, setRightDown, isPickDown, setPickDown 的代码保持不变) ...
bool Character::isLeftDown() const {
    return leftDown;
}

void Character::setLeftDown(bool leftDown) {
    Character::leftDown = leftDown;
}

bool Character::isRightDown() const {
    return rightDown;
}

void Character::setRightDown(bool rightDown) {
    Character::rightDown = rightDown;
}

bool Character::isPickDown() const {
    return pickDown;
}

void Character::setPickDown(bool pickDown) {
    Character::pickDown = pickDown;
}


const QPointF &Character::getVelocity() const {
    return velocity;
}

void Character::setVelocity(const QPointF &velocity) {
    Character::velocity = velocity;
}

void Character::processInput() {
    auto currentVelocity = velocity; // 获取当前速度
    currentVelocity.setX(0); // 重置水平速度
    const auto moveSpeed = 4.5;
    if (isLeftDown()) {
        currentVelocity.setX(currentVelocity.x() - moveSpeed);
        setTransform(QTransform().scale(1, 1));
    }
    if (isRightDown()) {
        currentVelocity.setX(currentVelocity.x() + moveSpeed);
        setTransform(QTransform().scale(-1, 1));
    }
    setVelocity(currentVelocity); // 设置新的速度

    if (!lastPickDown && pickDown) { // first time pickDown
        picking = true;
    } else {
        picking = false;
    }
    lastPickDown = pickDown;
}

void Character::jump() {
    if (onGround) {
        velocity.setY(JUMP_STRENGTH);
        onGround = false;
    }
}

void Character::applyGravity(qreal gravity) {
    velocity.setY(velocity.y() + gravity);
}

void Character::setOnGround(bool onGround) {
    this->onGround = onGround;
}

bool Character::isOnGround() const {
    return onGround;
}

bool Character::isPicking() const {
    return picking;
}

Armor *Character::pickupArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->unmount();
        oldArmor->setPos(newArmor->pos());
        oldArmor->setParentItem(parentItem());
    }
    newArmor->setParentItem(this);
    newArmor->mountToParent();
    armor = newArmor;
    return oldArmor;
}
