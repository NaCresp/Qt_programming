//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include "Character.h"
#include "../Weapon/Fist.h"

const qreal JUMP_STRENGTH = -15.0; // 定义跳跃力度
const qreal SQUAT_OFFSET_Y = 20.0; // 定义一个常量表示下蹲时的高度偏移

Character::Character(QObject *parent) : Item(parent, "")
{
    //    ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
    //    // Optionally, set some properties of the ellipse
    //    ellipseItem->setBrush(Qt::green);          // Fill color
    //    ellipseItem->setZValue(1);
    weapon = new Fist(this);
}

// ... (除了 squat 和 standUp 之外的其他函数保持不变) ...

bool Character::isLeftDown() const
{
    return leftDown;
}

void Character::setLeftDown(bool leftDown)
{
    Character::leftDown = leftDown;
}

bool Character::isRightDown() const
{
    return rightDown;
}

void Character::setRightDown(bool rightDown)
{
    Character::rightDown = rightDown;
}

bool Character::isPickDown() const
{
    return pickDown;
}

bool Character::isAttackDown() const
{
    return attackDown;
}

void Character::setAttackDown(bool attackDown)
{
    Character::attackDown = attackDown;
}

void Character::setSquatDown(bool squatDown)
{
    this->squatDown = squatDown;
}

bool Character::isSquatting() const
{
    return isSquattingState;
}

// --- 核心修正在这里 ---
void Character::squat()
{
    if (isSquattingState) return;
    isSquattingState = true;

    // 修正: 不再移动整个角色，只移动上半身组件
    // moveBy(0, SQUAT_OFFSET_Y); // <-- 删除此行

    // 将头部和盔甲相对于角色向下移动，实现“压缩”效果
    if (headEquipment)
    {
        headEquipment->moveBy(0, SQUAT_OFFSET_Y);
    }
    if (armor)
    {
        armor->moveBy(0, SQUAT_OFFSET_Y);
    }

    // 切换腿部贴图
    if (legEquipment)
    {
        legEquipment->setSquatMode(true);
    }
}

void Character::standUp()
{
    if (!isSquattingState) return;
    isSquattingState = false;

    // 修正: 不再移动整个角色，只移动上半身组件
    // moveBy(0, -SQUAT_OFFSET_Y); // <-- 删除此行

    // 将头部和盔甲也移回它们在角色身上的原始相对位置
    if (headEquipment)
    {
        headEquipment->moveBy(0, -SQUAT_OFFSET_Y);
    }
    if (armor)
    {
        armor->moveBy(0, -SQUAT_OFFSET_Y);
    }

    // 恢复腿部贴图
    if (legEquipment)
    {
        legEquipment->setSquatMode(false);
    }
}
// --- 核心修改结束 ---

void Character::attack()
{
    if (weapon)
    {
        weapon->attack();
    }
}

void Character::setPickDown(bool pickDown)
{
    Character::pickDown = pickDown;
}

const QPointF &Character::getVelocity() const
{
    return velocity;
}

void Character::setVelocity(const QPointF &velocity)
{
    Character::velocity = velocity;
}

void Character::processInput()
{
    // 处理下蹲状态切换
    if (squatDown)
    {
        squat();
    }
    else
    {
        standUp();
    }

    auto currentVelocity = velocity; // 获取当前速度
    currentVelocity.setX(0);         // 重置水平速度
    const auto moveSpeed = 4.5;

    // 如果不在下蹲，才允许左右移动
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

    setVelocity(currentVelocity); // 设置新的速度

    if (!lastPickDown && pickDown)
    { // first time pickDown
        picking = true;
    }
    else
    {
        picking = false;
    }
    lastPickDown = pickDown;

    if (!lastAttackDown && attackDown)
    { // 第一次按下攻擊鍵
        attack();
    }
    lastAttackDown = attackDown;
}

void Character::jump()
{
    // 下蹲时不允许跳跃
    if (onGround && !isSquattingState)
    {
        velocity.setY(JUMP_STRENGTH);
        onGround = false;
    }
}

void Character::applyGravity(qreal gravity)
{
    velocity.setY(velocity.y() + gravity);
}

void Character::setOnGround(bool onGround)
{
    this->onGround = onGround;
}

bool Character::isOnGround() const
{
    return onGround;
}

bool Character::isPicking() const
{
    return picking;
}

Armor *Character::pickupArmor(Armor *newArmor)
{
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
