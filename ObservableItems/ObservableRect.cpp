#include "ObservableItems/ObservableRect.h"

ObservableRect::ObservableRect(const QRectF& rect, QGraphicsItem* parent)
    : QGraphicsRectItem(rect, parent) {
    w = rect.width();
    h = rect.height();
    pointType = None;
    kx = ky = 1.0;
    alpha = 0.0;

    setAcceptHoverEvents(true);
}

QRectF ObservableRect::boundingRect() const {
    const qreal border = 1;
    return QRectF(-border, -border, w + 2 * border, h + 2 * border);
}

void ObservableRect::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
    event->accept();
    previousPosition = event->scenePos();
    if (isSelected()) {
        if (event->button() == Qt::LeftButton) {
            startMousePoint = event->scenePos();
            startItemPoint = scenePos();
            pointType = pointTypeAtPos(event->pos());
            setCursorShape(event->pos());
            kx0 = kx;
            ky0 = ky;
            alpha0 = alpha;

            QPointF p0;
            switch (pointType) {
            case (ResizeLeftTop):
                p0 = QPointF(w, h);
                break;
            case (ResizeTop):
                p0 = QPointF(w / 2, h);
                break;
            case (ResizeRightTop):
                p0 = QPointF(0, h);
                break;
            case (ResizeLeft):
                p0 = QPointF(w, h / 2);
                break;
            case (ResizeRight):
                p0 = QPointF(0, h / 2);
                break;
            case (ResizeLeftBottom):
                p0 = QPointF(w, 0);
                break;
            case (ResizeBottom):
                p0 = QPointF(w / 2, 0);
                break;
            case (ResizeRightBottom):
                p0 = QPointF(0, 0);
                break;
            default:;
            }
            p0 = mapToScene(p0);

            transform.reset();
            transform.rotateRadians(-alpha0);
            transform.translate(-p0.x(), -p0.y());
            transform_inv = transform.inverted(nullptr);
        }
    }
}

void ObservableRect::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseMoveEvent(event);

    if (!isSelected()) return;

    if (pointType == None) return;

    if (pointType == Move) {
        QPointF p = startItemPoint + event->scenePos() - startMousePoint;
        setPos(mapToParent(mapFromScene(p)));
        return;
    }

    if (pointType != Rotate) {
        QPointF pm = transform.map(startMousePoint);
        QPointF pi = transform.map(startItemPoint);
        QPointF pp = transform.map(event->scenePos());

        qreal kxx = pp.x() / pm.x();
        qreal kyy = pp.y() / pm.y();

        switch (pointType) {
        case (ResizeTop):
        case (ResizeBottom):
            kxx = 1;
            break;
        case (ResizeLeft):
        case (ResizeRight):
            kyy = 1;
            break;
        default:;
        }

        kx = kx0 * kxx;
        ky = ky0 * kyy;

        qreal xx = pi.x() * kxx;
        qreal yy = pi.y() * kyy;

        QTransform tr;
        tr.rotateRadians(alpha0);
        tr.scale(kx, ky);

        setTransform(tr);
        setPos(mapToParent(mapFromScene(transform_inv.map(QPointF(xx, yy)))));
        return;
    }
    if (pointType == Rotate) {
        QPointF p1 = transform.map(startMousePoint);
        QPointF p2 = transform.map(event->scenePos());

        alpha = alpha0 + atan2(p2.y(), p2.x()) - atan2(p1.y(), p1.x());

        QTransform tr;
        tr.rotateRadians(alpha);
        tr.scale(kx, ky);
        setTransform(tr);
        return;
    }
}

void ObservableRect::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    if (isSelected()) {
        setCursorShape(event->pos());
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void ObservableRect::setCursorShape(const QPointF& pos) {
    switch (pointTypeAtPos(pos)) {
    case (ResizeLeftTop):
        setCursor(Qt::SizeFDiagCursor);
        break;
    case (ResizeTop):
        setCursor(Qt::SizeVerCursor);
        break;
    case (ResizeRightTop):
        setCursor(Qt::SizeBDiagCursor);
        break;
    case (ResizeLeft):
        setCursor(Qt::SizeHorCursor);
        break;
    case (ResizeRight):
        setCursor(Qt::SizeHorCursor);
        break;
    case (ResizeLeftBottom):
        setCursor(Qt::SizeBDiagCursor);
        break;
    case (ResizeBottom):
        setCursor(Qt::SizeVerCursor);
        break;
    case (ResizeRightBottom):
        setCursor(Qt::SizeFDiagCursor);
        break;
    case (Rotate):
        setCursor(Qt::SizeAllCursor);
        break;
    case (Move):
        setCursor(Qt::OpenHandCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
    }
}

inline static qreal norm(qreal x, qreal y) { return x * x + y * y; }

ObservableRect::PointType ObservableRect::pointTypeAtPos(
    const QPointF& pos) const {
    qreal x = pos.x();
    qreal y = pos.y();
    qreal r = 100;
    qDebug() << x << y;

    if (norm(x, y) < r) return ResizeLeftTop;
    if (norm(x - w, y) < r) return ResizeRightTop;
    if (norm(x, y - h) < r) return ResizeLeftBottom;
    if (norm(x - w, y - h) < r) return ResizeRightBottom;

    if (norm(x, y - h / 2) < r) return ResizeLeft;
    if (norm(x - w, y - h / 2) < r) return ResizeRight;
    if (norm(x - w / 2, y) < r) return ResizeTop;
    if (norm(x - w / 2, y - h) < r) return ResizeBottom;

    return Move;
}

void ObservableRect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
    event->accept();
    if (event->scenePos() == previousPosition &&
        !(flags() & QGraphicsItem::ItemIsMovable)) {
        QString name = data(Qt::UserRole).toString();
        QString description = "No description";
        QString FileName = "Settings/";
        FileName += data(Qt::UserRole).toString();
        FileName += ".txt";
        QString tmpDescription = "";
        QFile file(FileName);
        qDebug() << name;
        qDebug() << FileName;
        bool key = false;
        if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
            while (!file.atEnd()) {
                QString s = file.readLine();
                QStringList lst = s.split(" ");
                if (lst.at(0) == "name" && lst.at(1) == "=") {
                    name.clear();
                    for (int i = 7; i < s.size(); i++) {
                        name += s[i];
                    }
                } else {
                    tmpDescription += s;
                    key = true;
                }
            }
        } else {
            qDebug() << "Can't open file";
        }
        if (key) {
            description = tmpDescription;
        }
        file.close();
        StatsClickInc(name);
        QMessageBox* msgBox =
            new QMessageBox(QMessageBox::Information, name, description,
                            QMessageBox::Ok | QMessageBox::Cancel);
        msgBox->setButtonText(QMessageBox::Ok, "Print");
        int n = msgBox->exec();
        if (n == QMessageBox::Ok) {
            QList<QGraphicsItem*> items = scene()->items();
            QString tmp;
            for (auto i : items) {
                if (dynamic_cast<QGraphicsEllipseItem*>(i)) {
                    ObservableCircle* ellipse =
                        qgraphicsitem_cast<ObservableCircle*>(i);
                    switch (ellipse->CurrentDirection) {
                    case (ObservableCircle::Direction::Right): {
                        if (i->scenePos().y() > scenePos().y()) {
                            tmp += "1";
                        } else {
                            tmp += "0";
                        }
                        break;
                    }
                    case (ObservableCircle::Direction::Down): {
                        if (i->scenePos().x() > scenePos().x()) {
                            tmp += "1";
                        } else {
                            tmp += "0";
                        }
                        break;
                    }
                    case (ObservableCircle::Direction::Left): {
                        if (i->scenePos().y() < scenePos().y()) {
                            tmp += "1";
                        } else {
                            tmp += "0";
                        }
                        break;
                    }
                    case (ObservableCircle::Direction::Up): {
                        if (i->scenePos().x() < scenePos().x()) {
                            tmp += "1";
                        } else {
                            tmp += "0";
                        }
                        break;
                    }
                    }
                }
            }
            StatsPrintInc(name);
            tmp += "9";
            BCGen(tmp, name);
        }
        delete msgBox;
    }
}

void ObservableRect::BCGen(QString number, QString name) {
    BarcodePrinter* printer = new BarcodePrinter();
    // List available printers
    qDebug() << "Available printers: " << printer->getAvailablePrinters();
    // Configure printer (no argument means it prints pdf)
    QString PName;
    QFile file("Settings/PrinterName.txt");
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        PName = file.readLine();
    } else {
        qDebug() << "Printer name not found";
        LogPrint("Printer name not found");
    }
    LogPrint(PName);
    printer->configurePrinter(PName);
    // Print barcode
    printer->printBarcode(number, name);
}
