#include <QTransform>
#include "Character.h"
#include "../Weapon/Fist.h"

const qreal JUMP_STRENGTH = -15.0;
const qreal SQUAT_OFFSET_Y = 20.0;

QRectF Character::boundingRect() const
{
    return QRectF(-50, -180, 100, 180);
}

Character::Character(QObject *parent) : Item(parent, "")
{
    weapon = new Fist(this);
    maxHp = 100;
    currentHp = maxHp;
    speedBuffIcon = nullptr;
    healthBuffIcon = nullptr; // 新增

    // --- 新增肾上腺素相关 ---
    adrenalineTimer = new QTimer(this);
    connect(adrenalineTimer, &QTimer::timeout, this, &Character::handleAdrenalineTick);
    // --- 新增结束 ---
}

// --- 新增代码：实现getter ---
Speed* Character::getSpeedBuffIcon() const
{
    return speedBuffIcon;
}
Health* Character::getHealthBuffIcon() const // 新增
{
    return healthBuffIcon;
}
// --- 新增代码结束 ---

Weapon* Character::pickupWeapon(Weapon* newWeapon)
{
    Weapon* oldWeapon = this->weapon;

    if (dynamic_cast<Fist*>(oldWeapon) == nullptr) {
        oldWeapon->unmount();
        oldWeapon->setPos(newWeapon->pos());
        oldWeapon->setParentItem(this->parentItem());
    } else {
        oldWeapon->setVisible(false);
        oldWeapon = nullptr;
    }

    this->weapon = newWeapon;
    this->weapon->setParentItem(this);
    this->weapon->mountToParent();
    this->weapon->hasDealtDamage = true;

    return oldWeapon;
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

void Character::applySpeedBuff()
{
    if (!isSpeedBuffed)
    {
        isSpeedBuffed = true;
        speedMultiplier = 1.8;

        if (!speedBuffIcon)
        {
            speedBuffIcon = new Speed(this);
            speedBuffIcon->mountToParent();
        }
        speedBuffIcon->setVisible(true);
    }
}

void Character::removeSpeedBuff()
{
    if (isSpeedBuffed)
    {
        isSpeedBuffed = false;
        speedMultiplier = 1.0;

        if (speedBuffIcon)
        {
            speedBuffIcon->setVisible(false);
        }
    }
}

// --- 新增肾上腺素逻辑 ---
void Character::applyAdrenalineBuff()
{
    // 立即并持续增加移速
    applySpeedBuff();

    // 显示持续回血图标并开始回血
    if (!healthBuffIcon) {
        healthBuffIcon = new Health(this);
        healthBuffIcon->mountToParent();
    }
    healthBuffIcon->setVisible(true);

    adrenalineTicks = 5; // 效果持续5秒
    if (!adrenalineTimer->isActive()) {
        adrenalineTimer->start(1000); // 每秒触发一次
    }
    handleAdrenalineTick(); // 立即回一次血
}

void Character::handleAdrenalineTick()
{
    if (adrenalineTicks <= 0) {
        adrenalineTimer->stop();
        if (healthBuffIcon) healthBuffIcon->setVisible(false);
        // 肾上腺素效果结束，移速恢复正常
        removeSpeedBuff();
        return;
    }
    heal(10);
    adrenalineTicks--;
}
// --- 新增结束 ---

bool Character::hasSpeedBuff() const
{
    return isSpeedBuffed;
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
    const auto moveSpeed = 4.5 * speedMultiplier;

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