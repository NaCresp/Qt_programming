//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include <QKeyEvent>
#include <QPen>
#include <QFont> // 包含头文件
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"

const qreal GRAVITY = 0.5; // 定义重力加速度

// --- 新增代码：定义血条的尺寸 ---
const qreal HP_BAR_WIDTH = 200.0;
const qreal HP_BAR_HEIGHT = 20.0;

BattleScene::BattleScene(QObject *parent) : Scene(parent)
{
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 1040, 656);
    map = new Battlefield();
    character = new Link();
    character2 = new Link(); // 创建第二个角色
    spareArmor = new FlamebreakerArmor();
    spareArmor2 = new FlamebreakerArmor(); // 为第二个角色创建备用护甲

    const qreal characterScale = 0.3; // 缩小值
    character->setScale(characterScale);
    character2->setScale(characterScale);

    addItem(map);
    addItem(character);
    addItem(character2); // 将第二个角色添加到场景
    addItem(spareArmor);
    addItem(spareArmor2); // 将第二个备用护甲添加到场景

    map->scaleToFitScene(this);
    character->setPos(map->getSpawnPos() - QPointF(100, 0));  // 调整第一个角色的出生点
    character2->setPos(map->getSpawnPos() + QPointF(100, 0)); // 设置第二个角色的出生点

    spareArmor->unmount();
    spareArmor->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.75, map->getFloorHeight());
    spareArmor2->unmount();
    spareArmor2->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.25, map->getFloorHeight());

    platforms.append(new QGraphicsRectItem(QRectF(300, 160, 405, 20), map)); // 顶部深色平台
    platforms.append(new QGraphicsRectItem(QRectF(192, 285, 240, 20), map)); // 左侧浅色平台
    platforms.append(new QGraphicsRectItem(QRectF(544, 300, 240, 20), map)); // 右侧木质平台

    // 设置平台颜色
    platforms[0]->setBrush(Qt::darkGray);
    platforms[1]->setBrush(QColor(173, 216, 230)); // 浅蓝色
    platforms[2]->setBrush(QColor(139, 69, 19));   // 棕色

    for (auto p : platforms)
    {
        p->setPen(Qt::NoPen); // 平台不需要边框
    }

    // 将 hidingZone 直接添加到场景，确保坐标系一致
    hidingZone = new QGraphicsRectItem(50, 380, 320, 20);
    hidingZone->setPen(Qt::NoPen);
    addItem(hidingZone);

    // --- 新增代码：初始化血条UI ---
    // 玩家1的血条
    player1HpBarBg = new QGraphicsRectItem(20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player1HpBar = new QGraphicsRectItem(20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player1HpText = new QGraphicsTextItem();

    // 玩家2的血条
    player2HpBarBg = new QGraphicsRectItem(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player2HpBar = new QGraphicsRectItem(width() - HP_BAR_WIDTH - 20, 10, HP_BAR_WIDTH, HP_BAR_HEIGHT);
    player2HpText = new QGraphicsTextItem();

    // 设置颜色
    player1HpBarBg->setBrush(Qt::darkGray);
    player1HpBar->setBrush(Qt::red);
    player2HpBarBg->setBrush(Qt::darkGray);
    player2HpBar->setBrush(Qt::red);

    // 设置字体和颜色
    QFont font("Arial", 12, QFont::Bold);
    player1HpText->setFont(font);
    player2HpText->setFont(font);
    player1HpText->setDefaultTextColor(Qt::white);
    player2HpText->setDefaultTextColor(Qt::white);

    // 设置文字位置（在血条下方）
    player1HpText->setPos(20, 10 + HP_BAR_HEIGHT);
    player2HpText->setPos(width() - HP_BAR_WIDTH - 20, 10 + HP_BAR_HEIGHT);

    // 设置Z值，确保前景在背景之上
    player1HpBarBg->setZValue(10);
    player1HpBar->setZValue(11);
    player1HpText->setZValue(12);
    player2HpBarBg->setZValue(10);
    player2HpBar->setZValue(11);
    player2HpText->setZValue(12);

    // 添加到场景
    addItem(player1HpBarBg);
    addItem(player1HpBar);
    addItem(player1HpText);
    addItem(player2HpBarBg);
    addItem(player2HpBar);
    addItem(player2HpText);

    updateHpDisplay(); // 初始化显示
}

void BattleScene::updateHpDisplay()
{
    if (!character || !character2) return;

    // 更新玩家1的血量
    int p1_hp = character->getCurrentHp();
    int p1_max_hp = character->getMaxHp();
    qreal p1_percent = (p1_max_hp > 0) ? (static_cast<qreal>(p1_hp) / p1_max_hp) : 0.0;
    // 根据百分比计算血条前景的宽度
    player1HpBar->setRect(20, 10, HP_BAR_WIDTH * p1_percent, HP_BAR_HEIGHT);
    player1HpText->setPlainText(QString("%1 / %2").arg(p1_hp).arg(p1_max_hp));


    // 更新玩家2的血量
    int p2_hp = character2->getCurrentHp();
    int p2_max_hp = character2->getMaxHp();
    qreal p2_percent = (p2_max_hp > 0) ? (static_cast<qreal>(p2_hp) / p2_max_hp) : 0.0;
    // 根据百分比计算血条前景的宽度
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

    // 第二个角色的操作
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

    // 第二个角色的操作
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

void BattleScene::update()
{
    Scene::update();
    updateHpDisplay(); // 在每一帧都更新血量显示
    processAttacks(); // 在每一帧都处理攻击检测
}

// --- 核心修改在这里 ---
void BattleScene::processAttacks()
{
    if (!character || !character2) return;

    // 获取两个角色的武器
    Weapon* weapon1 = character->getWeapon();
    Weapon* weapon2 = character2->getWeapon();

    // 构建角色的碰撞盒，减小宽度以缩小判定范围
    QRectF hitbox1(character->x() - 15, character->y() - 100, 30, 150);
    QRectF hitbox2(character2->x() - 15, character2->y() - 100, 30, 150);

    // 检测角色1的攻击
    // 增加 !weapon1->hasDealtDamage 条件，确保本次攻击只造成一次伤害
    if (weapon1 && weapon1->isVisible() && !weapon1->hasDealtDamage && weapon1->sceneBoundingRect().intersects(hitbox2))
    {
        character2->takeDamage(10); // 角色2受到伤害
        // 不再隐藏武器，而是将其标记为“已造成伤害”
        weapon1->hasDealtDamage = true;
    }

    // 检测角色2的攻击
    // 增加 !weapon2->hasDealtDamage 条件
    if (weapon2 && weapon2->isVisible() && !weapon2->hasDealtDamage && weapon2->sceneBoundingRect().intersects(hitbox1))
    {
        character->takeDamage(10); // 角色1受到伤害
        // 标记本次攻击已造成伤害
        weapon2->hasDealtDamage = true;
    }
}
// --- 核心修改结束 ---

void BattleScene::processMovement()
{
    Scene::processMovement();

    for (auto *currentChar : {this->character, this->character2})
    {
        if (!currentChar)
            continue;

        // 如果角色血量为0，则跳过移动处理 (可以添加死亡动画等)
        if (currentChar->getCurrentHp() <= 0)
        {
            // currentChar->setVisible(false); // 比如直接让角色消失
            continue;
        }

        // 1. 默认角色不在地面上，除非检测到碰撞
        currentChar->setOnGround(false);

        // 2. 对角色的垂直速度应用重力
        currentChar->applyGravity(GRAVITY);

        // 3. 计算角色在下一帧的理论位置
        const qreal estimatedCharWidth = 50;
        const qreal estimatedCharHeight = 150;
        QRectF charPhysicsRect = QRectF(currentChar->x() - estimatedCharWidth / 2, currentChar->y() - estimatedCharHeight, estimatedCharWidth, estimatedCharHeight);

        qreal feetY = currentChar->y();
        QPointF nextVelocity = currentChar->getVelocity() * (double)deltaTime / 15.0;
        QPointF nextPos = currentChar->pos() + nextVelocity;

        // 4. 确定角色脚下有效的地面 Y 坐标 (默认为地图底层)
        qreal effectiveFloorY = map->getFloorHeight();

        // 5. 检测是否将要降落在某个平台上
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

        // 6. 根据有效的地面，进行碰撞处理并更新最终位置
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

        // 隐藏区域逻辑:
        // 不直接隐藏角色本身(currentChar->setVisible(false))
        // 而是遍历其子项，隐藏除武器外的所有部分
        // 这样可以确保角色“隐身”，但武器在攻击时仍然可以正常显示和检测
        QRectF characterHitbox(currentChar->x() - 25, currentChar->y() - 100, 50, 150);
        bool isInHidingZone = hidingZone->rect().intersects(characterHitbox);
        bool shouldBeHidden = currentChar->isSquatting() && isInHidingZone;

        Weapon* weapon = currentChar->getWeapon();
        for (auto* childItem : currentChar->childItems()) {
            // 不要隐藏武器，武器的可见性由其自身的攻击逻辑控制
            if (childItem == weapon) {
                continue;
            }
            // 隐藏或显示角色的其他部分
            childItem->setVisible(!shouldBeHidden);
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
            spareArmor = dynamic_cast<Armor *>(pickupMountable(character, mountable));
        }
    }
    if (character2->isPicking())
    {
        auto mountable = findNearestUnmountedMountable(character2->pos(), 100.);
        if (mountable != nullptr)
        {
            spareArmor2 = dynamic_cast<Armor *>(pickupMountable(character2, mountable));
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
    return nullptr;
}
