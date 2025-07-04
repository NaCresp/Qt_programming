#include <QTransform>
#include "Character.h"
#include "../Weapon/Fist.h"
#include "../Weapon/RangedWeapon.h"


const qreal JUMP_STRENGTH = -15.0;
const qreal SQUAT_OFFSET_Y = 20.0;

QRectF Character::boundingRect() const
{
    return QRectF(-50, -180, 100, 180);
}

Character::Character(QObject *parent) : Item(parent, "")
{
    // --- 核心修正：统一初始拳头的装备逻辑 ---
    weapon = new Fist(this);
    // 确保初始的拳头也通过 mountToParent() 进行配置
    // 这会调用我们刚刚在 Fist 类中重写的新方法
    weapon->mountToParent();

    maxHp = 100;
    currentHp = maxHp;
    speedBuffIcon = nullptr;
    healthBuffIcon = nullptr;

    adrenalineTimer = new QTimer(this);
    connect(adrenalineTimer, &QTimer::timeout, this, &Character::handleAdrenalineTick);
}

Speed* Character::getSpeedBuffIcon() const
{
    return speedBuffIcon;
}
Health* Character::getHealthBuffIcon() const
{
    return healthBuffIcon;
}

void Character::pickupWeapon(Weapon* newWeapon)
{
    if (this->weapon) {
        // --- 核心修复：在销毁前，先断开父子关系 ---
        // 这可以防止在Qt事件循环的间隙中，其他代码仍然尝试访问这个即将被删除的子项
        this->weapon->setParentItem(nullptr);
        this->weapon->deleteLater();
    }

    this->weapon = newWeapon;
    this->weapon->setParentItem(this);
    this->weapon->mountToParent();
    this->weapon->hasDealtDamage = true;
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

void Character::applyAdrenalineBuff()
{
    applySpeedBuff();

    if (!healthBuffIcon) {
        healthBuffIcon = new Health(this);
        healthBuffIcon->mountToParent();
    }
    healthBuffIcon->setVisible(true);

    adrenalineTicks = 5; 
    if (!adrenalineTimer->isActive()) {
        adrenalineTimer->start(1000); 
    }
    handleAdrenalineTick(); 
}

void Character::handleAdrenalineTick()
{
    if (adrenalineTicks <= 0) {
        adrenalineTimer->stop();
        if (healthBuffIcon) healthBuffIcon->setVisible(false);
        removeSpeedBuff();
        return;
    }
    heal(10);
    adrenalineTicks--;
}

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
        checkWeaponAmmo();
    }
}

void Character::checkWeaponAmmo()
{
    if (auto rangedWeapon = dynamic_cast<RangedWeapon*>(weapon)) {
        if (rangedWeapon->getCurrentAmmo() <= 0) {
            rangedWeapon->deleteLater();
            
            weapon = new Fist(this);
            weapon->mountToParent(); 
        }
    }
}

void Character::setPickDown(bool pickDown) { Character::pickDown = pickDown; }

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