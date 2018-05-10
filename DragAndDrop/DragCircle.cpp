#include "DragAndDrop/DragCircle.h"

DragCircle::DragCircle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x, y, w, h, parent)
{
}

void DragCircle::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    QDrag *drag = new QDrag(this->scene());
    QMimeData *data = new QMimeData();
    data->setText("circle");
    drag->setMimeData(data);
    drag->start();
}
