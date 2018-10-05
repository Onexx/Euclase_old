#include <DragAndDrop/DropScene.h>

void DropScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText() &&
        (event->mimeData()->text() == "circle" ||
         event->mimeData()->text() == "rect")) {
        event->acceptProposedAction();
        event->accept();
    } else {
        QGraphicsScene::dragEnterEvent(event);
    }
}

void DropScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText() &&
        (event->mimeData()->text() == "circle" ||
         event->mimeData()->text() == "rect")) {
        event->acceptProposedAction();
        event->accept();
    } else {
        QGraphicsScene::dragMoveEvent(event);
    }
}

void DropScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText() &&
        (event->mimeData()->text() == "circle" ||
         event->mimeData()->text() == "rect")) {
        event->acceptProposedAction();
        event->accept();
    } else {
        QGraphicsScene::dragLeaveEvent(event);
    }
}

void DropScene::dropEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText() &&
        (event->mimeData()->text() == "circle" ||
         event->mimeData()->text() == "rect")) {
        event->acceptProposedAction();
        event->accept();
        if (event->mimeData()->text() == "circle") {
            addCircle(event->scenePos());
        } else {
            addRect(event->scenePos());
        }
    } else {
        QGraphicsScene::dropEvent(event);
    }
}

void DropScene::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> items = selectedItems();
        for (auto item : items) {
            delete item;
        }
    } else {
        QGraphicsScene::keyPressEvent(event);
    }
}

void DropScene::addCircle(const QPointF& pos) {
    int idx = 1;
    while (true) {
        bool exists = false;
        for (auto i : items()) {
            if (dynamic_cast<QGraphicsEllipseItem*>(i)) {
                if (i->data(Qt::UserRole).toInt() == idx) {
                    exists = true;
                    break;
                }
            }
        }
        if (exists)
            ++idx;
        else
            break;
    }
    auto Confw = 0, Confh = 0;
    bool key2 = true;
    QFile file("Settings/Conf.txt");
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            key2 = false;
            bool key = true;
            QString s = file.readLine();
            QStringList lst = s.split(" ");
            if (lst.at(0) == "WidthOfCircle" && lst.at(1) == "=") {
                Confw = lst.at(2).toInt(&key);
                key = false;
            } else {
                if (lst.at(0) == "HeightOfCircle" && lst.at(1) == "=") {
                    Confh = lst.at(2).toInt(&key);
                    key = false;
                } else {
                    if (lst.at(0) == "Colour" && lst.at(1) == "=") {
                        R = lst.at(2).toInt(&key);
                        G = lst.at(3).toInt(&key);
                        B = lst.at(4).toInt(&key);
                        key = false;
                    } else {
                        if (lst.at(0) == "HeightOfRectangle" ||
                            lst.at(0) == "WidthOfRectangle" ||
                            (s[0] == '/' && s[1] == '/')) {
                            key = false;
                        }
                    }
                }
            }
            if (key) {
                file.close();
                if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
                    file.write(
                        "//You can change width or height walues here, but "
                        "don't forget spaces\r\n"
                        "WidthOfRectangle = 50\r\n"
                        "HeightOfRectangle = 40\r\n"
                        "WidthOfCircle = 30\r\n"
                        "HeightOfCircle = 30\r\n"
                        "//Change colour walue in format R G B\r\n"
                        "Colour = 153 255 255\r\n");
                }
                Confh = 30;
                Confw = 30;
                qDebug() << "Rewriting the configurations";
                break;
            }
        }
    } else {
        qDebug() << "Can't open configurations file";
        Confh = 30;
        Confw = 30;
    }
    if (key2) {
        qDebug() << "Rewriting the configurations:Empty file";
        file.close();
        if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
            file.write(
                "//You can change width or height walues here, but don't "
                "forget spaces\r\n"
                "WidthOfRectangle = 50\r\n"
                "HeightOfRectangle = 40\r\n"
                "WidthOfCircle = 30\r\n"
                "HeightOfCircle = 30\r\n"
                "//Change colour walue in format R G B\r\n"
                "Colour = 153 255 255\r\n");
        }
        Confh = 30;
        Confw = 30;
    }
    file.close();
    QSizeF size(Confw, Confh);
    auto item = new ObservableCircle(QRectF(QPointF(), size));
    item->setData(Qt::UserRole, idx);
    auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
    auto dirline = new QGraphicsLineItem(item);
    dirline->setLine(item->rect().width(), item->rect().height() / 2,
                     item->rect().width() + 7, item->rect().height() / 2);
    dirline->setPen(
        QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    dirline->setData(Qt::UserRole, idx);  // Needs to be changed
    number->setFlag(QGraphicsItem::ItemIsSelectable, false);
    number->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    number->setPos(
        (item->rect().width() - number->boundingRect().width()) / 2,
        (item->rect().height() - number->boundingRect().height()) / 2);
    item->setPos(pos.x() - size.width() / 2, pos.y() - size.height() / 2);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setBrush(QBrush(QColor(R, G, B, 150)));  // colour
    addItem(item);
}

void DropScene::addRect(const QPointF& pos) {
    int idx = 1;
    while (true) {
        bool exists = false;
        for (auto i : items()) {
            if (dynamic_cast<QGraphicsRectItem*>(i)) {
                if (i->data(Qt::UserRole).toInt() == idx) {
                    exists = true;
                    break;
                }
            }
        }
        if (exists)
            ++idx;
        else
            break;
    }
    auto Confw = 0, Confh = 0;
    bool key2 = true;
    QFile file("Settings/Conf.txt");
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            key2 = false;
            bool key = true;
            QString s = file.readLine();
            QStringList lst = s.split(" ");
            if (lst.at(0) == "WidthOfRectangle" && lst.at(1) == "=") {
                Confw = lst.at(2).toInt(&key);
                key = false;
            } else {
                if (lst.at(0) == "HeightOfRectangle" && lst.at(1) == "=") {
                    Confh = lst.at(2).toInt(&key);
                    key = false;
                } else {
                    if (lst.at(0) == "Colour" && lst.at(1) == "=") {
                        R = lst.at(2).toInt(&key);
                        G = lst.at(3).toInt(&key);
                        B = lst.at(4).toInt(&key);
                        key = false;
                    } else {
                        if (lst.at(0) == "HeightOfCircle" ||
                            lst.at(0) == "WidthOfCircle" ||
                            (s[0] == '/' && s[1] == '/')) {
                            key = false;
                        }
                    }
                }
            }
            if (key) {
                file.close();
                if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
                    file.write(
                        "//You can change width or height walues here, but "
                        "don't forget spaces\r\n"
                        "WidthOfRectangle = 50\r\n"
                        "HeightOfRectangle = 40\r\n"
                        "WidthOfCircle = 30\r\n"
                        "HeightOfCircle = 30\r\n"
                        "//Change colour walue in format R G B\r\n"
                        "Colour = 153 255 255\r\n");
                }
                Confh = 50;
                Confw = 50;
                qDebug() << "Rewriting the configurations";
                break;
            }
        }
    } else {
        qDebug() << "Can't open configurations file";
        Confh = 50;
        Confw = 50;
    }
    if (key2) {
        qDebug() << "Rewriting the configurations:Empty file";
        file.close();
        if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
            file.write(
                "//You can change width or height walues here, but don't "
                "forget spaces\r\n"
                "WidthOfRectangle = 50\r\n"
                "HeightOfRectangle = 40\r\n"
                "WidthOfCircle = 30\r\n"
                "HeightOfCircle = 30\r\n"
                "//Change colour walue in format R G B\r\n"
                "Colour = 153 255 255\r\n");
        }
        Confh = 50;
        Confw = 50;
    }
    file.close();
    QSizeF size(Confw, Confh);
    auto item = new ObservableRect(QRectF(QPointF(), size));
    item->setData(Qt::UserRole, idx);
    auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
    number->setFlag(QGraphicsItem::ItemIsSelectable, false);
    number->setPos(
        (item->rect().width() - number->boundingRect().width()) / 2,
        (item->rect().height() - number->boundingRect().height()) / 2);
    item->setPos(pos.x() - size.width() / 2, pos.y() - size.height() / 2);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setBrush(QBrush(QColor(R, G, B, 150)));
    addItem(item);
}
