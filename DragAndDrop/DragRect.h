#pragma once

#include "MainWindow.h"
#include <QGraphicsEllipseItem>
#include <QMimeData>
#include <QDrag>
#include <QGraphicsSceneDragDropEvent>

class DragRect : public QGraphicsRectItem
{
public:
    explicit DragRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};
