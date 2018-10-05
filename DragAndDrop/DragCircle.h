#pragma once

#include <QDrag>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "Navig/Navig.h"

class DragCircle : public QGraphicsEllipseItem {
  public:
    explicit DragCircle(qreal x, qreal y, qreal w, qreal h,
                        QGraphicsItem* parent = nullptr);

  private:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
};
