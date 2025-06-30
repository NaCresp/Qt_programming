//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QGraphicsTextItem> // 包含头文件
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include <QGraphicsRectItem>

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

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void updateHpDisplay(); // 更新血量显示的私有方法
    void processAttacks();  // 处理攻击碰撞的私有方法
    Mountable *findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    static Mountable *pickupMountable(Character *character, Mountable *mountable);

    Map *map;
    Character *character;
    Character *character2; // 第二个角色
    Armor *spareArmor;
    Armor *spareArmor2; // 第二个角色的备用护甲
    QList<QGraphicsRectItem *> platforms;
    QGraphicsRectItem *hidingZone;

    // --- 新增代码：血条UI元素 ---
    QGraphicsRectItem *player1HpBarBg;      // 玩家1血条背景
    QGraphicsRectItem *player1HpBar;        // 玩家1血条前景
    QGraphicsTextItem *player1HpText;       // 玩家1血量数值文字
    QGraphicsRectItem *player2HpBarBg;      // 玩家2血条背景
    QGraphicsRectItem *player2HpBar;        // 玩家2血条前景
    QGraphicsTextItem *player2HpText;       // 玩家2血量数值文字
};

#endif // QT_PROGRAMMING_2024_BATTLESCENE_H
