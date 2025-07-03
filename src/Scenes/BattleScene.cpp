#include <QDebug>
#include <QKeyEvent>
#include <QPen>
#include <QFont>
#include <QBrush>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"
#include "../Items/Weapon/Knife.h"
#include "../Items/FloatingText.h"

const qreal GRAVITY = 0.5;
const qreal HP_BAR_WIDTH = 200.0;
const qreal HP_BAR_HEIGHT = 20.0;

BattleScene::BattleScene(QObject *parent) : Scene(parent)
{
    gameOver = false;

    setSceneRect(0, 0, 1040, 656);
    map = new Battlefield();
    character = new Link();
    character2 = new Link();
    spareArmor = new FlamebreakerArmor();
    spareArmor2 = new FlamebreakerArmor();
    spareWeapon = new Knife();

    map->setZValue(-10);

    const qreal characterScale = 0.3;
    character->setScale(characterScale);
    character2->setScale(characterScale);

    addItem(map);
    addItem(character);
    addItem(character2);
    addItem(spareArmor);
    addItem(spareArmor2);
    addItem(spareWeapon);

    map->scaleToFitScene(this);
    character->setPos(map->getSpawnPos() - QPointF(100, 0));
    character2->setPos(map->getSpawnPos() + QPointF(100, 0));

    spareArmor->unmount();
    spareArmor->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.75, map->getFloorHeight());
    spareArmor2->unmount();
    spareArmor2->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.25, map->getFloorHeight());

    spareWeapon->unmount();
    spareWeapon->setZValue(1);
    spareWeapon->setPos(sceneRect().center().x(), map->getFloorHeight() - 20);

    platforms.append(new QGraphicsRectItem(QRectF(300, 160, 405, 20), map));
    platforms.append(new QGraphicsRectItem(QRectF(192, 285, 240, 20), map));
    platforms.append(new QGraphicsRectItem(QRectF(544, 300, 240, 20), map));
    platforms[0]->setBrush(Qt::darkGray);
    platforms[1]->setBrush(QColor(173, 216, 230));
    platforms[2]->setBrush(QColor(139, 69, 19));
    for (auto p : platforms) { p->setPen(Qt::NoPen); }

    hidingZone = new QGraphicsRectItem(50, 380, 320, 20);
    hidingZone->setPen(Qt::NoPen);
    addItem(hidingZone);

    speedBuffZone = new QGraphicsRectItem(380, 410, 430, 5);
    // 用于调试区域
    // QColor buffColor(0, 0, 255, 80);
    // speedBuffZone->setBrush(QBrush(buffColor));

    speedBuffZone->setPen(Qt::NoPen);
    speedBuffZone->setZValue(-5);
    addItem(speedBuffZone);

    player1HpBarBg = new QGraphicsRectItem(20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player1HpBar = new QGraphicsRectItem(20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player1HpText = new QGraphicsTextItem();
    player2HpBarBg = new QGraphicsRectItem(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player2HpBar = new QGraphicsRectItem(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player2HpText = new QGraphicsTextItem();
    player1HpBarBg->setBrush(Qt::darkGray);
    player1HpBar->setBrush(Qt::red);
    player2HpBarBg->setBrush(Qt::darkGray);
    player2HpBar->setBrush(Qt::red);
    QFont font("Arial", 12, QFont::Bold);
    player1HpText->setFont(font);
    player2HpText->setFont(font);
    player1HpText->setDefaultTextColor(Qt::white);
    player2HpText->setDefaultTextColor(Qt::white);
    player1HpText->setPos(20, 10 + HP_BAR_HEIGHT);
    player2HpText->setPos(width() - HP_BAR_WIDTH - 20, 10 + HP_BAR_HEIGHT);
    player1HpBarBg->setZValue(10);
    player1HpBar->setZValue(11);
    player1HpText->setZValue(12);
    player2HpBarBg->setZValue(10);
    player2HpBar->setZValue(11);
    player2HpText->setZValue(12);
    addItem(player1HpBarBg);
    addItem(player1HpBar);
    addItem(player1HpText);
    addItem(player2HpBarBg);
    addItem(player2HpBar);
    addItem(player2HpText);
    updateHpDisplay();

    gameOverText = new QGraphicsTextItem();
    gameOverText->setFont(QFont("Arial", 50, QFont::Bold));
    gameOverText->setDefaultTextColor(Qt::yellow);
    gameOverText->setZValue(200);
    gameOverText->setVisible(false);
    addItem(gameOverText);

    connect(character, &Character::healthChanged, this, &BattleScene::showFloatingText);
    connect(character2, &Character::healthChanged, this, &BattleScene::showFloatingText);
}

void BattleScene::update()
{
    if (gameOver) {
        return;
    }

    Scene::update();
    updateHpDisplay();
    processAttacks();
    updateFloatingTexts();
    checkBuffs();

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
                p->removeSpeedBuff();
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
    QRectF hitbox1(character->x() - 15, character->y() - 100, 30, 150);
    QRectF hitbox2(character2->x() - 15, character2->y() - 100, 30, 150);
    if (weapon1 && weapon1->isVisible() && !weapon1->hasDealtDamage && weapon1->sceneBoundingRect().intersects(hitbox2))
    {
        character2->takeDamage(10);
        weapon1->hasDealtDamage = true;
    }
    if (weapon2 && weapon2->isVisible() && !weapon2->hasDealtDamage && weapon2->sceneBoundingRect().intersects(hitbox1))
    {
        character->takeDamage(10);
        weapon2->hasDealtDamage = true;
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
        
        // --- 核心修正：在循环中跳过对Buff图标的显隐控制 ---
        Weapon* weapon = currentChar->getWeapon();
        Speed* buffIcon = currentChar->getSpeedBuffIcon(); // 获取Buff图标

        for (auto* childItem : currentChar->childItems()) {
            // 如果是武器或者Buff图标，就跳过，不控制它的显隐
            if (childItem == weapon || childItem == buffIcon) {
                continue;
            }
            // 只控制装备的显隐
            childItem->setVisible(!shouldBeHidden);
        }
        // --- 修正结束 ---
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
Mountable *BattleScene::pickupMountable(Character *character, Mountable *mountable)
{
    if (auto armor = dynamic_cast<Armor *>(mountable))
    {
        return character->pickupArmor(armor);
    }
    if (auto weapon = dynamic_cast<Weapon *>(mountable))
    {
        character->pickupWeapon(weapon);
        return nullptr;
    }
    return nullptr;
}