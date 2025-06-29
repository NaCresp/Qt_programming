#include "StartMenuScene.h"

StartMenuScene::StartMenuScene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 1040, 656);
    // 创建开始按钮
    startButton = new QPushButton("Start Game");
    startButton->setFixedSize(200, 50);

    // 将按钮添加到场景中
    QGraphicsProxyWidget *proxy = addWidget(startButton);
    proxy->setPos((width() - startButton->width()) / 2 - 100, (height() - startButton->height()) / 2 - 30);

    // 连接按钮的点击信号到场景的startGame信号
    connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGame);
}
