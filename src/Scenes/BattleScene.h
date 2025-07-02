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
#include <QGraphicsRectItem>
#include <QDateTime> // <-- 包含 QDateTime

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
    // --- 新增代码：用于管理浮动文字 ---
    // 定义一个结构体来保存浮动文字及其创建时间
    struct FloatingTextInfo {
        FloatingText *item;
        qint64 creationTime;
    };
    QList<FloatingTextInfo> activeFloatingTexts; // 列表，用于存储所有活动的浮动文字
    void updateFloatingTexts(); // 新增一个私有方法来更新它们的状态
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
    QList<QGraphicsRectItem *> platforms;
    QGraphicsRectItem *hidingZone;

    QGraphicsRectItem *player1HpBarBg;
    QGraphicsRectItem *player1HpBar;
    QGraphicsTextItem *player1HpText;
    QGraphicsRectItem *player2HpBarBg;
    QGraphicsRectItem *player2HpBar;
    QGraphicsTextItem *player2HpText;
};

#endif // QT_PROGRAMMING_2024_BATTLESCENE_H