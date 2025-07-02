#include <QTransform>
#include "Character.h"
#include "../Weapon/Fist.h"

const qreal JUMP_STRENGTH = -15.0;
const qreal SQUAT_OFFSET_Y = 20.0;

Character::Character(QObject *parent) : Item(parent, "")
{
    weapon = new Fist(this);
    maxHp = 100;
    currentHp = maxHp;
}

Weapon* Character::pickupWeapon(Weapon* newWeapon)
{
    Weapon* oldWeapon = this->weapon;

    // 1. 如果旧武器不是 Fist，说明它是之前捡来的，可以被丢弃
    if (dynamic_cast<Fist*>(oldWeapon) == nullptr) {
        oldWeapon->unmount();
        // 将旧武器丢在新武器的位置上
        oldWeapon->setPos(newWeapon->pos());
        // 将旧武器的父项从角色(this)切换回场景(this->parentItem())，防止被错误删除
        oldWeapon->setParentItem(this->parentItem());
    } else {
        // 2. 如果旧武器是 Fist，我们不丢弃它，只是让它不可见，为将来切换回来做准备
        oldWeapon->setVisible(false);
        // 返回值设为 nullptr，因为 Fist 不会真的“掉落”
        oldWeapon = nullptr;
    }

    // 3. 装备新武器
    this->weapon = newWeapon;
    this->weapon->setParentItem(this); // 新武器的父项是角色
    this->weapon->mountToParent();     // 执行装备动作

    this->weapon->hasDealtDamage = true;

    return oldWeapon; // 返回被替换掉的武器（如果是Fist则返回nullptr）
}


Armor* Character::pickupArmor(Armor *newArmor)
{
    heal(25);

    auto oldArmor = armor;
    if (oldArmor != nullptr)
    {
        oldArmor->unmount();
        oldArmor->setPos(newArmor->pos());
        oldArmor->setParentItem(parentItem());
    }
    newArmor->setParentItem(this);
    newArmor->mountToParent();
    armor = newArmor;
    return oldArmor;
}

bool Character::isLeftDown() const { return leftDown; }
void Character::setLeftDown(bool leftDown) { Character::leftDown = leftDown; }
bool Character::isRightDown() const { return rightDown; }
void Character::setRightDown(bool rightDown) { Character::rightDown = rightDown; }
bool Character::isPickDown() const { return pickDown; }
bool Character::isAttackDown() const { return attackDown; }
void Character::setAttackDown(bool attackDown) { Character::attackDown = attackDown; }
void Character::setSquatDown(bool squatDown) { this->squatDown = squatDown; }
bool Character::isSquatting() const { return isSquattingState; }

void Character::squat()
{
    if (isSquattingState) return;
    isSquattingState = true;
    if (headEquipment) { headEquipment->moveBy(0, SQUAT_OFFSET_Y); }
    if (armor) { armor->moveBy(0, SQUAT_OFFSET_Y); }
    if (legEquipment) { legEquipment->setSquatMode(true); }
}

void Character::standUp()
{
    if (!isSquattingState) return;
    isSquattingState = false;
    if (headEquipment) { headEquipment->moveBy(0, -SQUAT_OFFSET_Y); }
    if (armor) { armor->moveBy(0, -SQUAT_OFFSET_Y); }
    if (legEquipment) { legEquipment->setSquatMode(false); }
}

void Character::attack()
{
    if (weapon)
    {
        weapon->attack();
    }
}

void Character::setPickDown(bool pickDown) { Character::pickDown = pickDown; }
const QPointF &Character::getVelocity() const { return velocity; }
void Character::setVelocity(const QPointF &velocity) { Character::velocity = velocity; }

void Character::processInput()
{
    if (squatDown) { squat(); }
    else { standUp(); }

    auto currentVelocity = velocity;
    currentVelocity.setX(0);
    const auto moveSpeed = 4.5;

    if (!isSquattingState)
    {
        if (isLeftDown())
        {
            currentVelocity.setX(currentVelocity.x() - moveSpeed);
            setTransform(QTransform().scale(1, 1));
        }
        if (isRightDown())
        {
            currentVelocity.setX(currentVelocity.x() + moveSpeed);
            setTransform(QTransform().scale(-1, 1));
        }
    }

    setVelocity(currentVelocity);

    if (!lastPickDown && pickDown) { picking = true; }
    else { picking = false; }
    lastPickDown = pickDown;

    if (!lastAttackDown && attackDown) { attack(); }
    lastAttackDown = attackDown;
}

void Character::jump()
{
    if (onGround && !isSquattingState)
    {
        velocity.setY(JUMP_STRENGTH);
        onGround = false;
    }
}

void Character::applyGravity(qreal gravity) { velocity.setY(velocity.y() + gravity); }
void Character::setOnGround(bool onGround) { this->onGround = onGround; }
bool Character::isOnGround() const { return onGround; }
bool Character::isPicking() const { return picking; }

void Character::takeDamage(int amount)
{
    currentHp -= amount;
    if (currentHp < 0) { currentHp = 0; }
    emit healthChanged(-amount, scenePos());
}

void Character::heal(int amount)
{
    currentHp += amount;
    if (currentHp > maxHp) { currentHp = maxHp; }
    emit healthChanged(amount, scenePos());
}

int Character::getCurrentHp() const { return currentHp; }
int Character::getMaxHp() const { return maxHp; }
Weapon *Character::getWeapon() const { return weapon; }