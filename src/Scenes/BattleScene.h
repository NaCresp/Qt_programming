//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QGraphicsTextItem>
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include "../Items/Medicine/Bandage.h" // 新增
#include "../Items/Medicine/Kit.h"     // 新增
#include "../Items/Medicine/Adrenaline.h" // 新增
#include <QGraphicsRectItem>
#include <QDateTime> // <-- 包含 QDateTime
#include "../Items/Weapon/Bullet.h"

// 前向声明 FloatingText 类
class FloatingText;

class BattleScene : public Scene
{
    Q_OBJECT

public:
    explicit BattleScene(QObject *parent);

    void processInput() override;
    void processMovement() override;
    void processPicking() override;

protected slots:
    void update() override;
    void showFloatingText(int amount, const QPointF &position);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    // --- 新增代码：用于游戏结束逻辑 ---
    bool gameOver;                          // 游戏结束标志
    QGraphicsTextItem *gameOverText;        // 显示游戏结束信息的文本
    void checkGameOver();                   // 检查游戏是否结束的函数
    // --- 新增代码结束 ---

    // --- 用于管理浮动文字 ---
    struct FloatingTextInfo {
        FloatingText *item;
        qint64 creationTime;
    };
    QList<FloatingTextInfo> activeFloatingTexts;
    void updateFloatingTexts();
    // --- 管理结束 ---

    // --- 新增代码: Buff 区域 ---
    QGraphicsRectItem *speedBuffZone;
    void checkBuffs();
    // --- 新增代码结束 ---

    void updateHpDisplay();
    void processAttacks();
    Mountable *findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    static Mountable *pickupMountable(Character *character, Mountable *mountable);

    Map *map;
    Character *character;
    Character *character2;
    Armor *spareArmor;
    Armor *spareArmor2;
    // --- 新增医疗物品 ---
    Bandage *bandage;
    Kit *kit;
    Adrenaline *adrenaline;
    // --- 新增结束 ---
    QList<QGraphicsRectItem *> platforms;
    QGraphicsRectItem *hidingZone;

    QGraphicsRectItem *player1HpBarBg;
    QGraphicsRectItem *player1HpBar;
    QGraphicsTextItem *player1HpText;
    QGraphicsRectItem *player2HpBarBg;
    QGraphicsRectItem *player2HpBar;
    QGraphicsTextItem *player2HpText;

    void updateBullets(); 
    QList<Bullet*> bullets;

    Weapon *spareWeapon;
    Weapon *sniper; 
    Weapon *rifle;  
};

#endif // QT_PROGRAMMING_2024_BATTLESCENE_H