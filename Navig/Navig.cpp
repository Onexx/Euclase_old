#include "Navig.h"

Navig::Navig(QWidget* parent) : QWidget(parent), ui(new Ui::Navig) {
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    connect(ui->PbBlock, &QPushButton::clicked, this, &Navig::Block);
    connect(ui->PbOpen, &QPushButton::clicked, this, &Navig::Open);
    connect(ui->PbOpenMap, &QPushButton::clicked, this, &Navig::OpenMap);
    connect(ui->PbSaveMap, &QPushButton::clicked, this, &Navig::SaveMap);
    scene = new DropScene();
    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->graphicsView_2->setAttribute(Qt::WA_AcceptTouchEvents);
    auto scene2 = new QGraphicsScene();
    ui->graphicsView_2->setScene(scene2);
    auto c = new DragCircle(0, 0, 100, 100);
    auto r = new DragRect(0, 150, 100, 100);
    scene2->addItem(c);
    scene2->addItem(r);
    StatisticsCreate();
    QFile Tfile("FSave.Etnm");
    Tfile.open(QIODevice::WriteOnly);
    Tfile.close();
}

void Navig::Open() {
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "",
                                               "*.png *.jpg *.jpeg *.bmp");
    scene->clear();
    QPixmap Map(str);
    QSize MapSize = ui->graphicsView->size();
    Map = Map.scaled(MapSize, Qt::KeepAspectRatio);
    scene->addPixmap(Map);
    Map = Map.scaled(MapSize, Qt::KeepAspectRatio);
    QFile file("FSave.Etnm");
    if (!(file.open(QIODevice::WriteOnly))) {
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_2);
    out << Map;
    file.close();
}

void Navig::SaveMap() {
    QFile Tfile("FSave.Etnm");
    if (!(Tfile.open(QIODevice::ReadOnly))) {
        return;
    }
    QDataStream in(&Tfile);
    in.setVersion(QDataStream::Qt_4_2);
    QPixmap Map;
    in >> Map;
    Tfile.close();
    QString SaveName = QFileDialog::getSaveFileName(
        this, tr("Save File"), "", tr("Euclase Navigation Map(*.Enm)"));
    QSize MapSize = ui->graphicsView->size();
    Map = Map.scaled(MapSize, Qt::KeepAspectRatio);
    QFile file(SaveName);
    if (!(file.open(QIODevice::WriteOnly))) {
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_2);
    int kol = 0;
    foreach (QGraphicsItem* item, ui->graphicsView->scene()->items()) {
        if (item->type() == QGraphicsEllipseItem::Type ||
            item->type() == QGraphicsRectItem::Type) {
            kol++;
        }
    }
    out << Map;
    qDebug() << "kol = " << kol;
    out << kol;
    foreach (QGraphicsItem* item, ui->graphicsView->scene()->items()) {
        if (item->type() == QGraphicsEllipseItem::Type) {
            QString Type = "ellipse";
            out << Type;
            int idx = item->data(Qt::UserRole).toInt(),
                w = item->boundingRect().width() - 1,
                h = item->boundingRect().height() - 1;
            QPointF pos = item->pos();
            ObservableCircle* ellipse =
                qgraphicsitem_cast<ObservableCircle*>(item);
            int dir = ellipse->CurrentDirection;
            out << w << h;
            out << pos;
            out << idx;
            out << dir;
            qDebug() << w << ' ' << h << ' ' << pos << ' ' << idx << ' ' << dir
                     << endl;
        }
        if (item->type() == QGraphicsRectItem::Type) {
            QString Type = "rect";
            out << Type;
            int idx = item->data(Qt::UserRole).toInt(),
                w = item->boundingRect().width() - 2,
                h = item->boundingRect().height() - 2;
            QPointF pos = item->pos();
            out << w << h;
            out << pos;
            out << idx;
            qDebug() << w << ' ' << h << ' ' << pos << ' ' << idx << endl;
        }
    }
    file.close();
}

void Navig::OpenMap() {
    qDebug() << "Opening the map";
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.Enm");
    QFile file(str);
    if (!(file.open(QIODevice::ReadOnly))) {
        qDebug() << "Error while opening map";
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_2);
    scene->clear();
    QPixmap Map;
    in >> Map;
    scene->addPixmap(Map);
    int kol;
    in >> kol;
    int R = 255, G = 255, B = 153;
    bool key2 = true;
    QFile Cfile("Settings/Conf.txt");
    if (Cfile.open(QIODevice::Text | QIODevice::ReadOnly)) {
        while (!Cfile.atEnd()) {
            bool key = true;
            key2 = false;
            QString s = Cfile.readLine();
            QStringList lst = s.split(" ");
            if (lst.at(0) == "Colour" && lst.at(1) == "=") {
                R = lst.at(2).toInt(&key);
                G = lst.at(3).toInt(&key);
                B = lst.at(4).toInt(&key);
                key = false;
            } else {
                if (lst.at(0) == "HeightOfCircle" ||
                    lst.at(0) == "WidthOfCircle" ||
                    lst.at(0) == "HeightOfRectangle" ||
                    lst.at(0) == "WidthOfRectangle" ||
                    (s[0] == '/' && s[1] == '/')) {
                    key = false;
                }
            }
            if (key) {
                QMessageBox(QMessageBox::Information, "Conf.txt",
                            "File written encorrectly, reutrning to standart "
                            "configurations",
                            QMessageBox::Ok);
                Cfile.close();
                if (Cfile.open(QIODevice::Text | QIODevice::WriteOnly)) {
                    Cfile.write(
                        "//You can change width or height walues here, but "
                        "don't forget spaces\r\n "
                        "WidthOfRectangle = 50\r\n"
                        "HeightOfRectangle = 40\r\n"
                        "WidthOfCircle = 30\r\n"
                        "HeightOfCircle = 30\r\n"
                        "//Change colour walue in format R G B\r\n"
                        "Colour = 153 255 255\r\n");
                }
                qDebug() << "Rewriting the configurations";
                break;
            }
        }
    } else {
        qDebug() << "Can't open configuratons file";
    }
    if (key2) {
        Cfile.close();
        if (Cfile.open(QIODevice::Text | QIODevice::WriteOnly)) {
            Cfile.write(
                "//You can change width or height walues here, but don't "
                "forget spaces\r\n"
                "WidthOfRectangle = 50\r\n"
                "HeightOfRectangle = 40\r\n"
                "WidthOfCircle = 30\r\n"
                "HeightOfCircle = 30\r\n"
                "//Change colour walue in format R G B\r\n"
                "Colour = 153 255 255\r\n");
        }
        qDebug() << "Rewriting the configurations";
    }
    Cfile.close();
    qDebug() << "Loading items";
    qDebug() << kol;
    for (int i = 0; i < kol; i++) {
        QString Type;
        in >> Type;
        qDebug() << "Type = " << Type;
        if (Type == "ellipse") {
            QPointF pos;
            int w, h, idx, dir;
            in >> w >> h;
            in >> pos;
            in >> idx;
            in >> dir;
            qDebug() << w << h << pos << idx;
            QSizeF size(w, h);
            auto item = new ObservableCircle(QRectF(QPointF(), size));
            item->setData(Qt::UserRole, idx);
            auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
            auto dirline = new QGraphicsLineItem(item);
            dirline->setPen(
                QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            dirline->setData(Qt::UserRole, item->data(Qt::UserRole));
            switch (dir) {
            case (0): {
                dirline->setLine(
                    item->rect().width(), item->rect().height() / 2,
                    item->rect().width() + 7, item->rect().height() / 2);
                break;
            }
            case (1): {
                dirline->setLine(
                    item->rect().width() / 2, item->rect().height(),
                    item->rect().width() / 2, item->rect().height() + 7);
                break;
            }
            case (2): {
                dirline->setLine(0, item->rect().height() / 2, -7,
                                 item->rect().height() / 2);
                break;
            }
            case (3): {
                dirline->setLine(item->rect().width() / 2, 0,
                                 item->rect().width() / 2, -7);
                break;
            }
            }
            number->setFlag(QGraphicsItem::ItemIsSelectable, false);
            number->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
            number->setPos(
                (item->rect().width() - number->boundingRect().width()) / 2,
                (item->rect().height() - number->boundingRect().height()) / 2);
            item->setPos(pos.x(), pos.y());
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setBrush(QBrush(QColor(R, G, B, 150)));  // colour
            scene->addItem(item);
        }
        if (Type == "rect") {
            QPointF pos;
            int w, h, idx;
            in >> w >> h;
            in >> pos;
            in >> idx;
            qDebug() << w << ' ' << h << ' ' << pos << ' ' << idx << endl;
            QSizeF size(w, h);
            auto item = new ObservableRect(QRectF(QPointF(), size));
            item->setData(Qt::UserRole, idx);
            auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
            number->setFlag(QGraphicsItem::ItemIsSelectable, false);
            number->setPos(
                (item->rect().width() - number->boundingRect().width()) / 2,
                (item->rect().height() - number->boundingRect().height()) / 2);
            item->setPos(pos.x(), pos.y());
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setBrush(QBrush(QColor(R, G, B, 150)));
            scene->addItem(item);
        }
    }

    file.close();
    QSize MapSize = ui->graphicsView->size();
    Map = Map.scaled(MapSize, Qt::KeepAspectRatio);
    QFile fileS("FSave.Etnm");
    if (!(fileS.open(QIODevice::WriteOnly))) {
        return;
    }
    QDataStream out(&fileS);
    out.setVersion(QDataStream::Qt_4_2);
    out << Map;
    foreach (QGraphicsItem* item, ui->graphicsView->scene()->items()) {
        if (item->type() == QGraphicsEllipseItem::Type ||
            item->type() == QGraphicsRectItem::Type) {
            kol++;
        }
    }
    out << kol;
    foreach (QGraphicsItem* item, ui->graphicsView->scene()->items()) {
        if (item->type() == QGraphicsEllipseItem::Type) {
            QString Type = "ellipse";
            out << Type;
            int w = item->boundingRect().width(),
                h = item->boundingRect().height();
            out << w << h;
            out << item->pos();
            out << item->data(Qt::UserRole).toInt();
        }
        if (item->type() == QGraphicsRectItem::Type) {
            QString Type = "rect";
            out << Type;
            int w = item->boundingRect().width(),
                h = item->boundingRect().height();
            out << w << h;
            out << item->pos();
            out << item->data(Qt::UserRole).toInt();
        }
    }
    fileS.close();
}

void Navig::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape && isBlocked) {
        isBlocked = false;
        ui->PbBlock->show();
        ui->PbOpen->show();
        ui->PbOpenMap->show();
        ui->PbSaveMap->show();
        ui->graphicsView_2->show();
        int kol = 0;
        for (auto item : ui->graphicsView->scene()->items()) {
            if (dynamic_cast<QGraphicsEllipseItem*>(item) ||
                dynamic_cast<QGraphicsRectItem*>(item)) {
                item->setFlag(QGraphicsItem::ItemIsMovable, true);
                qDebug() << kol++;
                item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            }
        }
    } else {
        if (event->key() == Qt::Key_S) {
            if (event->modifiers() & Qt::CTRL) {
                SaveMap();
                qDebug() << "Save";
            }
        } else {
            QWidget::keyPressEvent(event);
        }
    }
}

void Navig::Block() {
    isBlocked = true;
    QMessageBox* msgBox =
        new QMessageBox(QMessageBox::Information, "Blocking",
                        "Editing will be available by pressing Esc button",
                        QMessageBox::Ok | QMessageBox::Cancel);
    int n = msgBox->exec();
    delete msgBox;
    if (n == QMessageBox::Ok) {
        ui->PbBlock->hide();
        ui->PbOpen->hide();
        ui->PbOpenMap->hide();
        ui->PbSaveMap->hide();
        ui->graphicsView_2->hide();
        for (auto item : ui->graphicsView->scene()->items()) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        }
    }
}

Navig::~Navig() {}
