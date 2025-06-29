//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_MYGAME_H
#define QT_PROGRAMMING_2024_MYGAME_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QStackedWidget>
#include "Scenes/Scene.h"
#include "Scenes/StartMenuScene.h"
#include "Scenes/BattleScene.h"

class MyGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);

private slots:
    void showGameScene();

private:
    QStackedWidget *stackedWidget;
    QGraphicsView *view;
    StartMenuScene *startMenuScene;
    BattleScene *battleScene;
};

#endif // QT_PROGRAMMING_2024_MYGAME_H
