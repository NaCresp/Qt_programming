//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QTimer> // 确保 QTimer 被包含
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include "../Items/Medicine/Bandage.h"
#include "../Items/Medicine/Kit.h"
#include "../Items/Medicine/Adrenaline.h"
#include <QGraphicsRectItem>
#include <QDateTime>
#include "../Items/Weapon/Bullet.h"

// 前向声明 FloatingText 类
class FloatingText;

class BattleScene : public Scene
{
    Q_OBJECT

public:
    explicit BattleScene(QObject *parent);

    // --- 新增：为 BattleScene 重写 startLoop ---
    void startLoop();

    void processInput() override;
    void processMovement() override;
    void processPicking() override;

protected slots:
    void update() override;
    void showFloatingText(int amount, const QPointF &position);
    void spawnRandomItem(); // 新增的槽函数，用于生成物品
    void updateShieldDisplay(); // 新增

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    // --- 游戏结束逻辑 ---
    bool gameOver;
    QGraphicsTextItem *gameOverText;
    void checkGameOver();

    // --- 管理浮动文字 ---
    struct FloatingTextInfo {
        FloatingText *item;
        qint64 creationTime;
    };
    QList<FloatingTextInfo> activeFloatingTexts;
    void updateFloatingTexts();

    // --- Buff 区域 ---
    QGraphicsRectItem *speedBuffZone;
    void checkBuffs();

    void updateHpDisplay();
    void updateAmmoDisplay();
    void processAttacks();
    Mountable *findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    Mountable *pickupMountable(Character *character, Mountable *mountable);

    Map *map;
    Character *character;
    Character *character2;

    QList<QGraphicsRectItem *> platforms;
    QGraphicsRectItem *hidingZone;

    // UI元素
    QGraphicsRectItem *player1HpBarBg;
    QGraphicsRectItem *player1HpBar;
    QGraphicsTextItem *player1HpText;
    QGraphicsRectItem *player2HpBarBg;
    QGraphicsRectItem *player2HpBar;
    QGraphicsTextItem *player2HpText;

    QGraphicsRectItem *player1AmmoBarBg;
    QGraphicsRectItem *player1AmmoBar;
    QGraphicsTextItem *player1AmmoText;
    QGraphicsRectItem *player2AmmoBarBg;
    QGraphicsRectItem *player2AmmoBar;
    QGraphicsTextItem *player2AmmoText;

    // 新增护盾UI
    QGraphicsRectItem *player1ShieldBarBg;
    QGraphicsRectItem *player1ShieldBar;
    QGraphicsTextItem *player1ShieldText;
    QGraphicsRectItem *player2ShieldBarBg;
    QGraphicsRectItem *player2ShieldBar;
    QGraphicsTextItem *player2ShieldText;


    void updateBullets();
    QList<Bullet*> bullets;
    
    QTimer *itemDropTimer;
};

#endif // QT_PROGRAMMING_2024_BATTLESCENE_H