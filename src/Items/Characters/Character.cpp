#include <QTransform>
#include "Character.h"
#include "../Weapon/Fist.h"
#include "../Weapon/RangedWeapon.h"
#include "../Weapon/Knife.h"
#include "../Weapon/Ball.h" // <-- 新增
#include "Link.h"
#include "../Armors/LightArmor.h"
#include "../Armors/HeavyArmor.h"
#include "../Armors/body.h"


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
    // 拾取新护甲时，旧护甲直接销毁
    if (this->armor) {
        // 如果旧护甲是默认的 OldShirt，不要把它丢到地上
        if (dynamic_cast<OldShirt*>(this->armor) == nullptr) {
            // 在删除前断开父子关系
            this->armor->setParentItem(nullptr);
        }
        this->armor->deleteLater();
        this->armor = nullptr;
    }

    this->armor = newArmor;
    this->armor->setParentItem(this);
    this->armor->mountToParent();
    
    // 如果是重甲，连接信号
    if (auto heavyArmor = dynamic_cast<HeavyArmor*>(newArmor)) {
        connect(heavyArmor, &HeavyArmor::shieldBroken, this, &Character::onShieldBroken);
        connect(heavyArmor, &HeavyArmor::shieldChanged, this, &Character::shieldChanged);
    }
    emit shieldChanged(); // 通知UI更新

    return nullptr; // 原护甲已销毁，返回空
}

void Character::onShieldBroken()
{
    if (armor) {
        armor->deleteLater();
        armor = nullptr;
    }
    // 换上默认的身体
    armor = new OldShirt(this);
    armor->mountToParent();
    emit shieldChanged(); // 通知UI更新
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
    bool replaceWeapon = false;
    if (auto rangedWeapon = dynamic_cast<RangedWeapon*>(weapon)) {
        if (rangedWeapon->getCurrentAmmo() <= 0) {
            replaceWeapon = true;
        }
    } else if (auto ballWeapon = dynamic_cast<Ball*>(weapon)) { // <-- 新增
        if (ballWeapon->getCurrentAmmo() <= 0) {
            replaceWeapon = true;
        }
    }

    if (replaceWeapon) {
        weapon->deleteLater();
        weapon = new Fist(this);
        weapon->mountToParent();
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

void Character::takeDamage(int amount, Weapon *sourceWeapon)
{
    int finalDamage = amount;

    // 轻甲逻辑
    if (auto lightArmor = dynamic_cast<LightArmor*>(armor)) {
        if (dynamic_cast<Fist*>(sourceWeapon)) {
            finalDamage = 0; // 免疫拳头伤害
        } else if (dynamic_cast<Knife*>(sourceWeapon)) {
            finalDamage -= 5; // 小刀伤害减5
            if (finalDamage < 0) finalDamage = 0;
        }
    }
    // 重甲逻辑
    else if (auto heavyArmor = dynamic_cast<HeavyArmor*>(armor)) {
        if (dynamic_cast<RangedWeapon*>(sourceWeapon) || dynamic_cast<Ball*>(sourceWeapon)) { // <-- 新增
            int shieldDamage = finalDamage * 0.5;
            int playerDamage = finalDamage - shieldDamage;
            heavyArmor->takeShieldDamage(shieldDamage);
            finalDamage = playerDamage;
        }
    }

    if (finalDamage > 0) {
        currentHp -= finalDamage;
        if (currentHp < 0) { currentHp = 0; }
        emit healthChanged(-finalDamage, scenePos());
    }
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
Armor *Character::getArmor() const { return armor; }