#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include "Navig/Navig.h"

class ObservableCircle : public QGraphicsEllipseItem {
  public:
    enum Direction { Right, Down, Left, Up };
    Direction CurrentDirection;

    explicit ObservableCircle(const QRectF& rect,
                              QGraphicsItem* parent = nullptr);

  private:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void DirectionDrawing(Direction& dir);

    void BCGen(int number, const QPointF& pos);

    QPointF previousPosition;
};
