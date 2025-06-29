//
// Created by gerw on 8/20/24.
//
#include "MyGame.h"

MyGame::MyGame(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建开始菜单场景和战斗场景
    startMenuScene = new StartMenuScene(this);
    battleScene = new BattleScene(this);

    // 创建一个 QGraphicsView
    view = new QGraphicsView(this);
    view->setFixedSize(1040, 656);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建 QStackedWidget 并添加场景
    stackedWidget = new QStackedWidget(this);
    QWidget *startMenuWidget = new QWidget;
    QGraphicsView *startMenuView = new QGraphicsView(startMenuScene, startMenuWidget);
    startMenuView->setFixedSize(1200, 720);
    startMenuView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    startMenuView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 将战斗场景设置给 view
    view->setScene(battleScene);

    stackedWidget->addWidget(startMenuView);
    stackedWidget->addWidget(view);

    // 将 QStackedWidget 设置为中央窗口
    setCentralWidget(stackedWidget);
    setFixedSize(stackedWidget->sizeHint());

    // 连接信号和槽
    connect(startMenuScene, &StartMenuScene::startGame, this, &MyGame::showGameScene);

    // 初始显示开始菜单
    stackedWidget->setCurrentWidget(startMenuView);
}

void MyGame::showGameScene()
{
    // 切换到游戏场景
    stackedWidget->setCurrentWidget(view);
    battleScene->startLoop();
}
