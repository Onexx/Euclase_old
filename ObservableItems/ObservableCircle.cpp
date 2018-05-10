#include "ObservableItems/ObservableCircle.h"

ObservableCircle::ObservableCircle(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsEllipseItem(rect, parent)
{
    this->CurrentDirection = Right;
}

void ObservableCircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
    event->accept();
    previousPosition = event->scenePos();
}

void ObservableCircle::DirectionDrawing(Direction &dir){
    QList<QGraphicsItem *> items = scene()->items();
    for(auto i: items){
        auto line = dynamic_cast<QGraphicsLineItem*>(i);
        if(line->data(Qt::UserRole) == this->data(Qt::UserRole)){
            delete line;
        }
    }
    auto dirline = new QGraphicsLineItem(this);
    dirline->setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    dirline->setData(Qt::UserRole, this->data(Qt::UserRole));
    switch ( dir ){
    case(Right) : dirline->setLine(this->rect().width(), this->rect().height()/2, this->rect().width()+7, this->rect().height()/2);
    case(Down)  : dirline->setLine(this->rect().width()/2, 0, this->rect().width()/2, -7);
    case(Left)  : dirline->setLine(0, this->rect().height()/2, -7, this->rect().height()/2);
    case(Up)    : dirline->setLine(this->rect().width()/2, this->rect().height(), this->rect().width()/2, this->rect().height()+7);
    }
    qDebug()<<this->data(Qt::UserRole)<<' '<<dirline->data(Qt::UserRole)<<' '<<dir;
    scene()->addItem(dirline);
}

void ObservableCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    event->accept();
    if(event->scenePos() == previousPosition && (flags() & QGraphicsItem::ItemIsMovable)){
        switch( CurrentDirection )
        {
         case( Right ): CurrentDirection = Down;  break;
         case( Down ) : CurrentDirection = Left;  break;
         case( Left ) : CurrentDirection = Up;    break;
         case( Up )   : CurrentDirection = Right; break;
        }
    }
    DirectionDrawing(CurrentDirection);
}
