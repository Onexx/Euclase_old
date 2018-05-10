#include "DragAndDrop/DragRect.h"

DragRect::DragRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
{
}

void DragRect::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    QDrag *drag = new QDrag(this->scene());
    QMimeData *data = new QMimeData();
    data->setText("rect");
    drag->setMimeData(data);
    drag->start();
}
