#ifndef STARTMENUSCENE_H
#define STARTMENUSCENE_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>

class StartMenuScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit StartMenuScene(QObject *parent = nullptr);

signals:
    void startGame();

private:
    QPushButton *startButton;
};

#endif // STARTMENUSCENE_H
