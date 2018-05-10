#pragma once

#include "MainWindow.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>

class DropScene : public QGraphicsScene
{
    Q_OBJECT
public:
    qreal R=153,G=255,B=255;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void addCircle(const QPointF &pos);
    void addRect(const QPointF &pos);
};
