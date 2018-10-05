#pragma once

#include <QDrag>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "Navig/Navig.h"

class DragRect : public QGraphicsRectItem {
  public:
    explicit DragRect(qreal x, qreal y, qreal w, qreal h,
                      QGraphicsItem* parent = nullptr);

  private:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
};
