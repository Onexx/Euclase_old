#pragma once

#include <QDebug>
#include <QDrag>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QMimeData>
#include "Navig/Navig.h"

class DropScene : public QGraphicsScene {
    Q_OBJECT
  public:
    int R = 153, G = 255, B = 255;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

  private:
    void addCircle(const QPointF& pos);
    void addRect(const QPointF& pos);
};
