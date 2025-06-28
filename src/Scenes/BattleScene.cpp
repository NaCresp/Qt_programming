//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"

BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 1280, 720);
    map = new Battlefield();
    character = new Link();
    character2 = new Link(); // 创建第二个角色
    spareArmor = new FlamebreakerArmor();
    spareArmor2 = new FlamebreakerArmor(); // 为第二个角色创建备用护甲

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
    if (character != nullptr) {
        character->setPos(character->pos() + character->getVelocity() * (double) deltaTime);
    }
    if (character2 != nullptr) { // 处理第二个角色的移动
        character2->setPos(character2->pos() + character2->getVelocity() * (double) deltaTime);
    }
}

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
