#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include "BarcodePrinter/BarcodePrinter.h"
#include "Navig/Navig.h"
#include "Statistics/Logs.h"

class ObservableRect : public QGraphicsRectItem {
  public:
    explicit ObservableRect(const QRectF& rect, QGraphicsItem* parent = 0);
    enum PointType {
        None,
        ResizeLeftTop,
        ResizeTop,
        ResizeRightTop,
        ResizeLeft,
        ResizeRight,
        ResizeLeftBottom,
        ResizeBottom,
        ResizeRightBottom,
        Rotate,
        Move
    };

    QRectF boundingRect() const Q_DECL_OVERRIDE;

  private:
    PointType pointTypeAtPos(const QPointF& pos) const;
    void setCursorShape(const QPointF& pos);

    qreal w, h;

    PointType pointType;
    QPointF startItemPoint;
    QPointF startMousePoint;
    qreal kx, ky, alpha;
    qreal kx0, ky0, alpha0;
    QTransform transform, transform_inv;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void BCGen(QString number, QString name);
    QPointF previousPosition;

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) Q_DECL_OVERRIDE;
};
