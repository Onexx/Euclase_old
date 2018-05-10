#pragma once

#include "MainWindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMessageBox>

class ObservableCircle : public QGraphicsEllipseItem
{
public:

    enum Direction { Up, Right, Down, Left };
    Direction CurrentDirection;

    explicit ObservableCircle(const QRectF &rect, QGraphicsItem *parent = 0);
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void DirectionDrawing(Direction &dir);

    void BCGen(int number, const QPointF &pos);

    QPointF previousPosition;
};
