//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include <QKeyEvent> // 确保包含了 QKeyEvent
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"

const qreal GRAVITY = 0.5; // 定义重力加速度

BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 1040, 656);
    map = new Battlefield();
    character = new Link();
    character2 = new Link(); // 创建第二个角色
    spareArmor = new FlamebreakerArmor();
    spareArmor2 = new FlamebreakerArmor(); // 为第二个角色创建备用护甲

    const qreal characterScale = 0.3; //缩小值
    character->setScale(characterScale);
    character2->setScale(characterScale);

    addItem(map);
    addItem(character);
    addItem(character2); // 将第二个角色添加到场景
    addItem(spareArmor);
    addItem(spareArmor2); // 将第二个备用护甲添加到场景

    map->scaleToFitScene(this);
    character->setPos(map->getSpawnPos() - QPointF(100, 0)); // 调整第一个角色的出生点
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

    for(auto p : platforms) {
        p->setPen(Qt::NoPen); // 平台不需要边框
    }
}

void BattleScene::processInput() {
    Scene::processInput();
    if (character != nullptr) {
        character->processInput();
    }
    if (character2 != nullptr) { // 处理第二个角色的输入
        character2->processInput();
    }
}


void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_A:
        if (character != nullptr) {
            character->setLeftDown(true);
        }
        break;
    case Qt::Key_D:
        if (character != nullptr) {
            character->setRightDown(true);
        }
        break;
    case Qt::Key_J:
        if (character != nullptr) {
            character->setPickDown(true);
        }
        break;
    case Qt::Key_W: // 第一个角色的跳跃
        if (character != nullptr) {
            character->jump();
        }
        break;
    // 第二个角色的操作
    case Qt::Key_Left:
        if (character2 != nullptr) {
            character2->setLeftDown(true);
        }
        break;
    case Qt::Key_Right:
        if (character2 != nullptr) {
            character2->setRightDown(true);
        }
        break;
    case Qt::Key_1:
        if (character2 != nullptr) {
            character2->setPickDown(true);
        }
        break;
    case Qt::Key_Up: // 第二个角色的跳跃
        if (character2 != nullptr) {
            character2->jump();
        }
        break;
    default:
        Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_A:
        if (character != nullptr) {
            character->setLeftDown(false);
        }
        break;
    case Qt::Key_D:
        if (character != nullptr) {
            character->setRightDown(false);
        }
        break;
    case Qt::Key_J:
        if (character != nullptr) {
            character->setPickDown(false);
        }
        break;
    // 第二个角色的操作
    case Qt::Key_Left:
        if (character2 != nullptr) {
            character2->setLeftDown(false);
        }
        break;
    case Qt::Key_Right:
        if (character2 != nullptr) {
            character2->setRightDown(false);
        }
        break;
    case Qt::Key_1:
        if (character2 != nullptr) {
            character2->setPickDown(false);
        }
        break;
    default:
        Scene::keyReleaseEvent(event);
    }
}

void BattleScene::update() {
    Scene::update();
}

void BattleScene::processMovement() {
    Scene::processMovement();

    for (auto* character : {this->character, this->character2}) {
        if (!character) continue;

        // 1. 默认角色不在地面上，除非检测到碰撞
        character->setOnGround(false);

        // 2. 对角色的垂直速度应用重力
        character->applyGravity(GRAVITY);

        // 3. 计算角色在下一帧的理论位置
        QRectF charRect = character->sceneBoundingRect();
        qreal feetOffset = charRect.bottom() - character->pos().y();
        QPointF nextVelocity = character->getVelocity() * (double)deltaTime / 15.0;
        QPointF nextPos = character->pos() + nextVelocity;

        // 4. 确定角色脚下有效的地面 Y 坐标 (默认为地图底层)
        qreal effectiveFloorY = map->getFloorHeight();

        // 5. 检测是否将要降落在某个平台上
        if (character->getVelocity().y() >= 0) { // 只在下落时检测
            for (auto p : platforms) {
                QRectF pRect = p->sceneBoundingRect();

                // 检查水平方向是否重叠
                if (charRect.right() > pRect.left() && charRect.left() < pRect.right()) {
                    // 检查垂直方向是否即将发生碰撞 (当前在平台上方，下一帧将在平台下方)
                    if (charRect.bottom() <= pRect.top() && (charRect.bottom() + nextVelocity.y()) >= pRect.top()) {
                        // 如果这个平台比已知的地面要高，则更新有效地面
                        if (pRect.top() < effectiveFloorY) {
                            effectiveFloorY = pRect.top();
                        }
                    }
                }
            }
        }

        // 6. 根据有效的地面，进行碰撞处理并更新最终位置
        if ((character->pos().y() + feetOffset + nextVelocity.y()) >= effectiveFloorY) {
            // 将要穿过地面，修正位置，让角色恰好站在地面上
            character->setPos(nextPos.x(), effectiveFloorY - feetOffset);

            // 停止垂直下落并设置角色在地面上
            character->setVelocity(QPointF(character->getVelocity().x(), 0));
            character->setOnGround(true);
        } else {
            // 没有碰撞，正常移动
            character->setPos(nextPos);
        }
    }
}

// ... (processPicking, findNearestUnmountedMountable, 和 pickupMountable 保持不变) ...
void BattleScene::processPicking() {
    Scene::processPicking();
    if (character->isPicking()) {
        auto mountable = findNearestUnmountedMountable(character->pos(), 100.);
        if (mountable != nullptr) {
            spareArmor = dynamic_cast<Armor *>(pickupMountable(character, mountable));
        }
    }
    if (character2->isPicking()) { // 处理第二个角色的拾取
        auto mountable = findNearestUnmountedMountable(character2->pos(), 100.);
        if (mountable != nullptr) {
            spareArmor2 = dynamic_cast<Armor *>(pickupMountable(character2, mountable));
        }
    }
}

Mountable *BattleScene::findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold) {
    Mountable *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto mountable = dynamic_cast<Mountable *>(item)) {
            if (!mountable->isMounted()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = mountable;
                }
            }
        }
    }

    return nearest;
}

Mountable *BattleScene::pickupMountable(Character *character, Mountable *mountable) {
    // Limitation: currently only supports armor
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        return character->pickupArmor(armor);
    }
    return nullptr;
}
