#include <QDebug>
#include <QKeyEvent>
#include <QPen>
#include <QFont>
#include <QBrush>
#include <random> // 用于生成随机数
#include "BattleScene.h"
#include "../Items/Weapon/Fist.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"
#include "../Items/Medicine/Medicine.h"
#include "../Items/Weapon/Knife.h"
#include "../Items/FloatingText.h"
#include "../Items/Weapon/Sniper.h"
#include "../Items/Weapon/Rifle.h"
#include "../Items/Weapon/Bullet.h"
#include "../Items/Weapon/RangedWeapon.h"
#include "../Items/Medicine/Bandage.h"
#include "../Items/Medicine/Kit.h"
#include "../Items/Medicine/Adrenaline.h"
#include "../Items/Armors/LightArmor.h" // 新增
#include "../Items/Armors/HeavyArmor.h" // 新增


const qreal GRAVITY = 0.5;
const qreal HP_BAR_WIDTH = 200.0;
const qreal HP_BAR_HEIGHT = 20.0;
const qreal AMMO_BAR_WIDTH = 150.0;
const qreal AMMO_BAR_HEIGHT = 15.0;
const qreal SHIELD_BAR_WIDTH = 150.0;
const qreal SHIELD_BAR_HEIGHT = 15.0;

const qreal AMMO_BAR_Y_POS = 10 + HP_BAR_HEIGHT + 20;
const qreal SHIELD_BAR_Y_POS = AMMO_BAR_Y_POS + AMMO_BAR_HEIGHT + 20;


BattleScene::BattleScene(QObject *parent) : Scene(parent)
{
    gameOver = false;

    setSceneRect(0, 0, 1040, 656);
    map = new Battlefield();
    character = new Link();
    character2 = new Link();
    
    map->setZValue(-10);

    const qreal characterScale = 0.3;
    character->setScale(characterScale);
    character2->setScale(characterScale);

    addItem(map);
    addItem(character);
    addItem(character2);

    map->scaleToFitScene(this);
    character->setPos(map->getSpawnPos() - QPointF(100, 0));
    character2->setPos(map->getSpawnPos() + QPointF(100, 0));
    
    // 初始化物品掉落计时器，但先不启动
    itemDropTimer = new QTimer(this);
    connect(itemDropTimer, &QTimer::timeout, this, &BattleScene::spawnRandomItem);
    // --- 修改：移除此处的 itemDropTimer->start(5000); ---

    platforms.append(new QGraphicsRectItem(QRectF(300, 160, 405, 20), map));
    platforms.append(new QGraphicsRectItem(QRectF(192, 285, 240, 20), map));
    platforms.append(new QGraphicsRectItem(QRectF(544, 300, 240, 20), map));
    for (auto p : platforms) { p->setBrush(Qt::darkGray); p->setPen(Qt::NoPen); }

    hidingZone = new QGraphicsRectItem(50, 380, 320, 20);
    hidingZone->setPen(Qt::NoPen);
    addItem(hidingZone);

    speedBuffZone = new QGraphicsRectItem(380, 410, 430, 5);
    speedBuffZone->setPen(Qt::NoPen);
    speedBuffZone->setZValue(-5);
    addItem(speedBuffZone);

    // --- HP and Ammo Bars ---
    player1HpBarBg = new QGraphicsRectItem(20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player1HpBar = new QGraphicsRectItem(20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player1HpText = new QGraphicsTextItem();
    player1AmmoBarBg = new QGraphicsRectItem(20, AMMO_BAR_Y_POS, AMMO_BAR_WIDTH, AMMO_BAR_HEIGHT);
    player1AmmoBar = new QGraphicsRectItem(20, AMMO_BAR_Y_POS, AMMO_BAR_WIDTH, AMMO_BAR_HEIGHT);
    player1AmmoText = new QGraphicsTextItem();
    player1ShieldBarBg = new QGraphicsRectItem(20, SHIELD_BAR_Y_POS, SHIELD_BAR_WIDTH, SHIELD_BAR_HEIGHT);
    player1ShieldBar = new QGraphicsRectItem(20, SHIELD_BAR_Y_POS, SHIELD_BAR_WIDTH, SHIELD_BAR_HEIGHT);
    player1ShieldText = new QGraphicsTextItem();


    player2HpBarBg = new QGraphicsRectItem(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player2HpBar = new QGraphicsRectItem(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player2HpText = new QGraphicsTextItem();
    player2AmmoBarBg = new QGraphicsRectItem(width() - AMMO_BAR_WIDTH - 20, AMMO_BAR_Y_POS, AMMO_BAR_WIDTH, AMMO_BAR_HEIGHT);
    player2AmmoBar = new QGraphicsRectItem(width() - AMMO_BAR_WIDTH - 20, AMMO_BAR_Y_POS, AMMO_BAR_WIDTH, AMMO_BAR_HEIGHT);
    player2AmmoText = new QGraphicsTextItem();
    player2ShieldBarBg = new QGraphicsRectItem(width() - SHIELD_BAR_WIDTH - 20, SHIELD_BAR_Y_POS, SHIELD_BAR_WIDTH, SHIELD_BAR_HEIGHT);
    player2ShieldBar = new QGraphicsRectItem(width() - SHIELD_BAR_WIDTH - 20, SHIELD_BAR_Y_POS, SHIELD_BAR_WIDTH, SHIELD_BAR_HEIGHT);
    player2ShieldText = new QGraphicsTextItem();


    // Styling
    player1HpBarBg->setBrush(Qt::darkGray);
    player1HpBar->setBrush(Qt::red);
    player1AmmoBarBg->setBrush(Qt::darkGray);
    player1AmmoBar->setBrush(Qt::yellow);
    player1ShieldBarBg->setBrush(Qt::darkGray);
    player1ShieldBar->setBrush(Qt::cyan);


    player2HpBarBg->setBrush(Qt::darkGray);
    player2HpBar->setBrush(Qt::red);
    player2AmmoBarBg->setBrush(Qt::darkGray);
    player2AmmoBar->setBrush(Qt::yellow);
    player2ShieldBarBg->setBrush(Qt::darkGray);
    player2ShieldBar->setBrush(Qt::cyan);

    QFont font("Arial", 12, QFont::Bold);
    player1HpText->setFont(font);
    player1AmmoText->setFont(font);
    player1ShieldText->setFont(font);
    player2HpText->setFont(font);
    player2AmmoText->setFont(font);
    player2ShieldText->setFont(font);


    player1HpText->setDefaultTextColor(Qt::white);
    player1AmmoText->setDefaultTextColor(Qt::white);
    player1ShieldText->setDefaultTextColor(Qt::white);
    player2HpText->setDefaultTextColor(Qt::white);
    player2AmmoText->setDefaultTextColor(Qt::white);
    player2ShieldText->setDefaultTextColor(Qt::white);

    // Positioning
    player1HpText->setPos(20, 10 + HP_BAR_HEIGHT);
    player1AmmoText->setPos(20 + AMMO_BAR_WIDTH + 5, AMMO_BAR_Y_POS);
    player1ShieldText->setPos(20 + SHIELD_BAR_WIDTH + 5, SHIELD_BAR_Y_POS);
    player2HpText->setPos(width() - HP_BAR_WIDTH - 20, 10 + HP_BAR_HEIGHT);
    player2AmmoText->setPos(width() - AMMO_BAR_WIDTH - 20 - 50, AMMO_BAR_Y_POS);
    player2ShieldText->setPos(width() - SHIELD_BAR_WIDTH - 20 - 50, SHIELD_BAR_Y_POS);


    // Z-Value
    player1HpBarBg->setZValue(10);
    player1HpBar->setZValue(11);
    player1HpText->setZValue(12);
    player1AmmoBarBg->setZValue(10);
    player1AmmoBar->setZValue(11);
    player1AmmoText->setZValue(12);
    player1ShieldBarBg->setZValue(10);
    player1ShieldBar->setZValue(11);
    player1ShieldText->setZValue(12);

    player2HpBarBg->setZValue(10);
    player2HpBar->setZValue(11);
    player2HpText->setZValue(12);
    player2AmmoBarBg->setZValue(10);
    player2AmmoBar->setZValue(11);
    player2AmmoText->setZValue(12);
    player2ShieldBarBg->setZValue(10);
    player2ShieldBar->setZValue(11);
    player2ShieldText->setZValue(12);

    addItem(player1HpBarBg);
    addItem(player1HpBar);
    addItem(player1HpText);
    addItem(player1AmmoBarBg);
    addItem(player1AmmoBar);
    addItem(player1AmmoText);
    addItem(player1ShieldBarBg);
    addItem(player1ShieldBar);
    addItem(player1ShieldText);

    addItem(player2HpBarBg);
    addItem(player2HpBar);
    addItem(player2HpText);
    addItem(player2AmmoBarBg);
    addItem(player2AmmoBar);
    addItem(player2AmmoText);
    addItem(player2ShieldBarBg);
    addItem(player2ShieldBar);
    addItem(player2ShieldText);


    updateHpDisplay();
    updateAmmoDisplay();
    updateShieldDisplay();

    gameOverText = new QGraphicsTextItem();
    gameOverText->setFont(QFont("Arial", 50, QFont::Bold));
    gameOverText->setDefaultTextColor(Qt::yellow);
    gameOverText->setZValue(200);
    gameOverText->setVisible(false);
    addItem(gameOverText);

    connect(character, &Character::healthChanged, this, &BattleScene::showFloatingText);
    connect(character2, &Character::healthChanged, this, &BattleScene::showFloatingText);
    connect(character, &Character::shieldChanged, this, &BattleScene::updateShieldDisplay);
    connect(character2, &Character::shieldChanged, this, &BattleScene::updateShieldDisplay);
}

// --- 新增：实现 BattleScene 专属的 startLoop ---
void BattleScene::startLoop()
{
    // 首先，调用父类的 startLoop 来启动核心的游戏循环（用于移动、渲染等）
    Scene::startLoop();

    // 然后，只在这里启动物品掉落的计时器
    itemDropTimer->start(5000);
}

void BattleScene::spawnRandomItem()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 2); // 0: 药品, 1: 武器, 2: 护甲
    std::uniform_int_distribution<> medicineDist(0, 2); // 0: Bandage, 1: Kit, 2: Adrenaline
    std::uniform_int_distribution<> weaponDist(0, 2); // 0: Knife, 1: Rifle, 2: Sniper
    std::uniform_int_distribution<> armorDist(0, 1); // 0: LightArmor, 1: HeavyArmor
    std::uniform_real_distribution<> xPosDist(sceneRect().left() + 50, sceneRect().right() - 50);

    Item* newItem = nullptr;
    int itemType = typeDist(gen);

    if (itemType == 0) { // 生成药品
        int medicineType = medicineDist(gen);
        switch (medicineType) {
            case 0: newItem = new Bandage(); break;
            case 1: newItem = new Kit(); break;
            case 2: newItem = new Adrenaline(); break;
        }
    } else if (itemType == 1) { // 生成武器
        int weaponType = weaponDist(gen);
        switch (weaponType) {
            case 0: newItem = new Knife(); break;
            case 1: newItem = new Rifle(); break;
            case 2: newItem = new Sniper(); break;
        }
    } else { // 生成护甲
        int armorType = armorDist(gen);
        switch (armorType) {
            case 0: newItem = new LightArmor(); break;
            case 1: newItem = new HeavyArmor(); break;
        }
    }


    if (newItem) {
        if (auto mountable = dynamic_cast<Mountable*>(newItem)) {
            mountable->unmount();
        }
        qreal xPos = xPosDist(gen);
        newItem->setPos(xPos, sceneRect().top()); // 从天空掉落
        addItem(newItem);
    }
}


void BattleScene::update()
{
    if (gameOver) {
        return;
    }

    Scene::update();
    updateHpDisplay();
    updateAmmoDisplay();
    updateShieldDisplay(); // 新增
    processAttacks();
    updateFloatingTexts();
    checkBuffs();
    updateBullets();

    checkGameOver();
}

void BattleScene::checkBuffs()
{
    for (auto* p : {character, character2})
    {
        if (!p) continue;

        if (speedBuffZone->collidesWithItem(p))
        {
            p->applySpeedBuff();
        }
        else
        {
            if (p->hasSpeedBuff())
            {
                Health* healthIcon = p->getHealthBuffIcon();
                bool hasAdrenalineEffect = (healthIcon != nullptr) && healthIcon->isVisible();

                if (!hasAdrenalineEffect)
                {
                    p->removeSpeedBuff();
                }
            }
        }
    }
}


void BattleScene::checkGameOver() {
    if (gameOver || !character || !character2) {
        return;
    }

    QString winnerText;
    bool isGameOverNow = false;

    if (character->getCurrentHp() <= 0) {
        winnerText = "Player 2 Wins!";
        isGameOverNow = true;
    } else if (character2->getCurrentHp() <= 0) {
        winnerText = "Player 1 Wins!";
        isGameOverNow = true;
    }

    if (isGameOverNow) {
        updateHpDisplay();

        gameOver = true;
        itemDropTimer->stop(); // 游戏结束时停止生成物品
        gameOverText->setPlainText(winnerText);

        QRectF textRect = gameOverText->boundingRect();
        gameOverText->setPos((width() - textRect.width()) / 2, (height() - textRect.height()) / 2);
        gameOverText->setVisible(true);
    }
}


void BattleScene::showFloatingText(int amount, const QPointF &position)
{
    QString text;
    QColor color;

    if (amount < 0) {
        text = QString::number(amount);
        color = Qt::red;
    } else {
        text = "+" + QString::number(amount);
        color = Qt::green;
    }

    auto floatingText = new FloatingText(text, color);
    floatingText->setPos(position + QPointF(0, -100));
    addItem(floatingText);

    activeFloatingTexts.append({floatingText, QDateTime::currentMSecsSinceEpoch()});
}

void BattleScene::updateFloatingTexts()
{
    const qint64 DURATION = 3000;
    const qreal  SPEED = 40.0;

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    for (int i = activeFloatingTexts.size() - 1; i >= 0; --i)
    {
        FloatingTextInfo &info = activeFloatingTexts[i];
        qint64 age = currentTime - info.creationTime;

        if (age >= DURATION)
        {
            removeItem(info.item);
            delete info.item;
            activeFloatingTexts.removeAt(i);
        }
        else
        {
            qreal timeDelta = static_cast<qreal>(deltaTime) / 1000.0;
            info.item->moveBy(0, -SPEED * timeDelta);

            qreal opacity = 1.0 - (static_cast<qreal>(age) / DURATION);
            info.item->setOpacity(opacity);
        }
    }
}

void BattleScene::updateHpDisplay()
{
    if (!character || !character2) return;

    int p1_hp = character->getCurrentHp();
    int p1_max_hp = character->getMaxHp();
    qreal p1_percent = (p1_max_hp > 0) ? (static_cast<qreal>(p1_hp) / p1_max_hp) : 0.0;
    player1HpBar->setRect(20, 10, HP_BAR_WIDTH * p1_percent, HP_BAR_HEIGHT);
    player1HpText->setPlainText(QString("%1 / %2").arg(p1_hp).arg(p1_max_hp));

    int p2_hp = character2->getCurrentHp();
    int p2_max_hp = character2->getMaxHp();
    qreal p2_percent = (p2_max_hp > 0) ? (static_cast<qreal>(p2_hp) / p2_max_hp) : 0.0;
    player2HpBar->setRect(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH * p2_percent, HP_BAR_HEIGHT);
    player2HpText->setPlainText(QString("%1 / %2").arg(p2_hp).arg(p2_max_hp));
}

void BattleScene::updateAmmoDisplay()
{
    if (!character || !character2) return;

    // Player 1
    if (auto rangedWeapon = dynamic_cast<RangedWeapon*>(character->getWeapon())) {
        player1AmmoBarBg->setVisible(true);
        player1AmmoBar->setVisible(true);
        player1AmmoText->setVisible(true);
        int currentAmmo = rangedWeapon->getCurrentAmmo();
        int maxAmmo = rangedWeapon->getMaxAmmo();
        qreal percent = (maxAmmo > 0) ? (static_cast<qreal>(currentAmmo) / maxAmmo) : 0.0;
        player1AmmoBar->setRect(20, AMMO_BAR_Y_POS, AMMO_BAR_WIDTH * percent, AMMO_BAR_HEIGHT);
        player1AmmoText->setPlainText(QString("%1/%2").arg(currentAmmo).arg(maxAmmo));
    } else {
        player1AmmoBarBg->setVisible(false);
        player1AmmoBar->setVisible(false);
        player1AmmoText->setVisible(false);
    }

    // Player 2
    if (auto rangedWeapon = dynamic_cast<RangedWeapon*>(character2->getWeapon())) {
        player2AmmoBarBg->setVisible(true);
        player2AmmoBar->setVisible(true);
        player2AmmoText->setVisible(true);
        int currentAmmo = rangedWeapon->getCurrentAmmo();
        int maxAmmo = rangedWeapon->getMaxAmmo();
        qreal percent = (maxAmmo > 0) ? (static_cast<qreal>(currentAmmo) / maxAmmo) : 0.0;
        player2AmmoBar->setRect(width() - AMMO_BAR_WIDTH - 20, AMMO_BAR_Y_POS, AMMO_BAR_WIDTH * percent, AMMO_BAR_HEIGHT);
        player2AmmoText->setPlainText(QString("%1/%2").arg(currentAmmo).arg(maxAmmo));
    } else {
        player2AmmoBarBg->setVisible(false);
        player2AmmoBar->setVisible(false);
        player2AmmoText->setVisible(false);
    }
}

void BattleScene::updateShieldDisplay()
{
    if (!character || !character2) return;

    // Player 1
    if (auto heavyArmor = dynamic_cast<HeavyArmor*>(character->getArmor())) {
        player1ShieldBarBg->setVisible(true);
        player1ShieldBar->setVisible(true);
        player1ShieldText->setVisible(true);
        int currentShield = heavyArmor->getShield();
        int maxShield = heavyArmor->getMaxShield();
        qreal percent = (maxShield > 0) ? (static_cast<qreal>(currentShield) / maxShield) : 0.0;
        player1ShieldBar->setRect(20, SHIELD_BAR_Y_POS, SHIELD_BAR_WIDTH * percent, SHIELD_BAR_HEIGHT);
        player1ShieldText->setPlainText(QString("Shield: %1/%2").arg(currentShield).arg(maxShield));
    } else {
        player1ShieldBarBg->setVisible(false);
        player1ShieldBar->setVisible(false);
        player1ShieldText->setVisible(false);
    }

    // Player 2
    if (auto heavyArmor = dynamic_cast<HeavyArmor*>(character2->getArmor())) {
        player2ShieldBarBg->setVisible(true);
        player2ShieldBar->setVisible(true);
        player2ShieldText->setVisible(true);
        int currentShield = heavyArmor->getShield();
        int maxShield = heavyArmor->getMaxShield();
        qreal percent = (maxShield > 0) ? (static_cast<qreal>(currentShield) / maxShield) : 0.0;
        player2ShieldBar->setRect(width() - SHIELD_BAR_WIDTH - 20, SHIELD_BAR_Y_POS, SHIELD_BAR_WIDTH * percent, SHIELD_BAR_HEIGHT);
        player2ShieldText->setPlainText(QString("Shield: %1/%2").arg(currentShield).arg(maxShield));
    } else {
        player2ShieldBarBg->setVisible(false);
        player2ShieldBar->setVisible(false);
        player2ShieldText->setVisible(false);
    }
}



void BattleScene::processInput()
{
    Scene::processInput();
    if (character != nullptr)
    {
        character->processInput();
    }
    if (character2 != nullptr)
    {
        character2->processInput();
    }
}
void BattleScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A:
        if (character != nullptr)
        {
            character->setLeftDown(true);
        }
        break;
    case Qt::Key_D:
        if (character != nullptr)
        {
            character->setRightDown(true);
        }
        break;
    case Qt::Key_J:
        if (character != nullptr)
        {
            character->setPickDown(true);
        }
        break;
    case Qt::Key_W:
        if (character != nullptr)
        {
            character->jump();
        }
        break;
    case Qt::Key_K:
        if (character != nullptr)
        {
            character->setAttackDown(true);
        }
        break;
    case Qt::Key_S:
        if (character != nullptr)
        {
            character->setSquatDown(true);
        }
        break;
    case Qt::Key_Left:
        if (character2 != nullptr)
        {
            character2->setLeftDown(true);
        }
        break;
    case Qt::Key_Right:
        if (character2 != nullptr)
        {
            character2->setRightDown(true);
        }
        break;
    case Qt::Key_1:
        if (character2 != nullptr)
        {
            character2->setPickDown(true);
        }
        break;
    case Qt::Key_Up:
        if (character2 != nullptr)
        {
            character2->jump();
        }
        break;
    case Qt::Key_2:
        if (character2 != nullptr)
        {
            character2->setAttackDown(true);
        }
        break;
    case Qt::Key_Down:
        if (character2 != nullptr)
        {
            character2->setSquatDown(true);
        }
        break;
    default:
        Scene::keyPressEvent(event);
    }
}
void BattleScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A:
        if (character != nullptr)
        {
            character->setLeftDown(false);
        }
        break;
    case Qt::Key_D:
        if (character != nullptr)
        {
            character->setRightDown(false);
        }
        break;
    case Qt::Key_J:
        if (character != nullptr)
        {
            character->setPickDown(false);
        }
        break;
    case Qt::Key_K:
        if (character != nullptr)
        {
            character->setAttackDown(false);
        }
        break;
    case Qt::Key_S:
        if (character != nullptr)
        {
            character->setSquatDown(false);
        }
        break;
    case Qt::Key_Left:
        if (character2 != nullptr)
        {
            character2->setLeftDown(false);
        }
        break;
    case Qt::Key_Right:
        if (character2 != nullptr)
        {
            character2->setRightDown(false);
        }
        break;
    case Qt::Key_1:
        if (character2 != nullptr)
        {
            character2->setPickDown(false);
        }
        break;
    case Qt::Key_2:
        if (character2 != nullptr)
        {
            character2->setAttackDown(false);
        }
        break;
    case Qt::Key_Down:
        if (character2 != nullptr)
        {
            character2->setSquatDown(false);
        }
        break;
    default:
        Scene::keyReleaseEvent(event);
    }
}

void BattleScene::processAttacks()
{
    if (!character || !character2) return;

    Weapon* weapon1 = character->getWeapon();
    Weapon* weapon2 = character2->getWeapon();
    QRectF hitbox1(character->sceneBoundingRect());
    QRectF hitbox2(character2->sceneBoundingRect());

    if (auto meleeWeapon = dynamic_cast<Knife*>(weapon1)) {
        if (meleeWeapon && meleeWeapon->isVisible() && !meleeWeapon->hasDealtDamage && meleeWeapon->sceneBoundingRect().intersects(hitbox2))
        {
            character2->takeDamage(15, meleeWeapon);
            meleeWeapon->hasDealtDamage = true;
        }
    } else if (auto meleeWeapon = dynamic_cast<Fist*>(weapon1)) {
        if (meleeWeapon && meleeWeapon->isVisible() && !meleeWeapon->hasDealtDamage && meleeWeapon->sceneBoundingRect().intersects(hitbox2))
        {
            character2->takeDamage(10, meleeWeapon);
            meleeWeapon->hasDealtDamage = true;
        }
    }


    if (auto meleeWeapon = dynamic_cast<Knife*>(weapon2)) {
        if (meleeWeapon && meleeWeapon->isVisible() && !meleeWeapon->hasDealtDamage && meleeWeapon->sceneBoundingRect().intersects(hitbox1))
        {
            character->takeDamage(15, meleeWeapon);
            meleeWeapon->hasDealtDamage = true;
        }
    } else if (auto meleeWeapon = dynamic_cast<Fist*>(weapon2)) {
        if (meleeWeapon && meleeWeapon->isVisible() && !meleeWeapon->hasDealtDamage && meleeWeapon->sceneBoundingRect().intersects(hitbox1))
        {
            character->takeDamage(10, meleeWeapon);
            meleeWeapon->hasDealtDamage = true;
        }
    }
}

void BattleScene::processMovement()
{
    Scene::processMovement();
    for (auto *currentChar : {this->character, this->character2})
    {
        if (!currentChar)
            continue;
        if (currentChar->getCurrentHp() <= 0)
        {
            continue;
        }
        currentChar->setOnGround(false);
        currentChar->applyGravity(GRAVITY);
        const qreal estimatedCharWidth = 50;
        const qreal estimatedCharHeight = 150;
        QRectF charPhysicsRect = QRectF(currentChar->x() - estimatedCharWidth / 2, currentChar->y() - estimatedCharHeight, estimatedCharWidth, estimatedCharHeight);
        qreal feetY = currentChar->y();
        QPointF nextVelocity = currentChar->getVelocity() * (double)deltaTime / 15.0;
        QPointF nextPos = currentChar->pos() + nextVelocity;
        qreal effectiveFloorY = map->getFloorHeight();
        if (currentChar->getVelocity().y() >= 0)
        {
            for (auto p : platforms)
            {
                QRectF pRect = p->sceneBoundingRect();
                if (charPhysicsRect.right() > pRect.left() && charPhysicsRect.left() < pRect.right())
                {
                    if (feetY <= pRect.top() && (feetY + nextVelocity.y()) >= pRect.top())
                    {
                        if (pRect.top() < effectiveFloorY)
                        {
                            effectiveFloorY = pRect.top();
                        }
                    }
                }
            }
        }
        if ((feetY + nextVelocity.y()) >= effectiveFloorY)
        {
            currentChar->setPos(nextPos.x(), effectiveFloorY);
            currentChar->setVelocity(QPointF(currentChar->getVelocity().x(), 0));
            currentChar->setOnGround(true);
        }
        else
        {
            currentChar->setPos(nextPos);
        }
        QRectF characterHitbox(currentChar->x() - 25, currentChar->y() - 100, 50, 150);
        bool isInHidingZone = hidingZone->rect().intersects(characterHitbox);
        bool shouldBeHidden = currentChar->isSquatting() && isInHidingZone;
        
        Weapon* weapon = currentChar->getWeapon();
        Speed* buffIcon = currentChar->getSpeedBuffIcon();
        Health* healthIcon = currentChar->getHealthBuffIcon();

        for (auto* childItem : currentChar->childItems()) {
            if (childItem == weapon || childItem == buffIcon || childItem == healthIcon) {
                continue;
            }
            childItem->setVisible(!shouldBeHidden);
        }
    }
    
    // --- 终极修复版：为掉落的物品应用物理效果 ---
    for (QGraphicsItem *graphicsItem : items())
    {
        // 确保转换成功且对象没有父项（即它是场景的直接子项，是独立的掉落物）
        auto item = dynamic_cast<Item*>(graphicsItem);
        if (!item || item->parentItem() != nullptr) {
            continue;
        }

        // 进一步确保我们只处理“可挂载”的物品，比如武器和药品
        auto mountable = dynamic_cast<Mountable*>(item);
        if (!mountable || mountable->isMounted()) {
            continue;
        }

        item->applyGravity(GRAVITY);

        QPointF nextVelocity = item->getVelocity() * (double)deltaTime / 15.0;
        QPointF nextPos = item->pos() + nextVelocity;
        
        qreal effectiveFloorY = map->getFloorHeight();
        
        // 使用物品的包围盒来进行更精确的碰撞检测
        QRectF itemRect = item->sceneBoundingRect();

        if (item->getVelocity().y() >= 0) // 仅在下落时检测平台
        {
            for (auto p : platforms)
            {
                QRectF pRect = p->sceneBoundingRect();
                if (itemRect.right() > pRect.left() && itemRect.left() < pRect.right())
                {
                    // 检查物品的底部在下一帧是否会穿过平台的顶部
                    if (itemRect.bottom() <= pRect.top() && (itemRect.bottom() + nextVelocity.y()) >= pRect.top())
                    {
                        effectiveFloorY = pRect.top();
                        break;
                    }
                }
            }
        }

        // 检查与最终有效“地面”的碰撞
        if (itemRect.bottom() + nextVelocity.y() >= effectiveFloorY)
        {
            // 将物品的底部精确地放在地面/平台上
            item->setPos(nextPos.x(), effectiveFloorY - itemRect.height());
            item->setVelocity(QPointF(item->getVelocity().x(), 0)); // 停止下落
        }
        else
        {
            item->setPos(nextPos);
        }
    }
}
void BattleScene::processPicking()
{
    Scene::processPicking();
    if (character->isPicking())
    {
        auto mountable = findNearestUnmountedMountable(character->pos(), 100.);
        if (mountable != nullptr)
        {
            pickupMountable(character, mountable);
        }
    }
    if (character2->isPicking())
    {
        auto mountable = findNearestUnmountedMountable(character2->pos(), 100.);
        if (mountable != nullptr)
        {
            pickupMountable(character2, mountable);
        }
    }
}
Mountable *BattleScene::findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold)
{
    Mountable *nearest = nullptr;
    qreal minDistance = distance_threshold;
    for (QGraphicsItem *item : items())
    {
        if (auto mountable = dynamic_cast<Mountable *>(item))
        {
            if (!mountable->isMounted())
            {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance)
                {
                    minDistance = distance;
                    nearest = mountable;
                }
            }
        }
    }
    return nearest;
}


Mountable* BattleScene::pickupMountable(Character *character, Mountable *mountable)
{
    if (auto armor = dynamic_cast<Armor *>(mountable))
    {
        character->pickupArmor(armor);
        return nullptr; // 护甲已被装备或销毁，地上不再有东西
    }
    if (auto weapon = dynamic_cast<Weapon *>(mountable))
    {
        character->pickupWeapon(weapon);
        return nullptr;
    }
    if (auto medicine = dynamic_cast<Medicine *>(mountable))
    {
        medicine->applyEffect(character);
        medicine->deleteLater();
        return nullptr;
    }
    return nullptr;
}

void BattleScene::updateBullets()
{
    bullets.clear();
    for (QGraphicsItem* item : items()) {
        if (auto bullet = dynamic_cast<Bullet*>(item)) {
            bullets.append(bullet);
        }
    }

    for (int i = bullets.size() - 1; i >= 0; --i)
    {
        Bullet *bullet = bullets[i];
        bullet->updatePosition();

        bool hit = false;
        Character* owner = bullet->getOwner();
        Weapon* sourceWeapon = owner ? owner->getWeapon() : nullptr;

        if (owner == character2 && bullet->collidesWithItem(character))
        {
            character->takeDamage(bullet->getDamage(), sourceWeapon);
            hit = true;
        }
        else if (owner == character && bullet->collidesWithItem(character2))
        {
            character2->takeDamage(bullet->getDamage(), sourceWeapon);
            hit = true;
        }

        if (hit || !sceneRect().contains(bullet->pos()))
        {
            removeItem(bullet);
            delete bullet;
            bullets.removeAt(i);
        }
    }
}