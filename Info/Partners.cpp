#include "Partners.h"

Partners::Partners(QWidget *parent) : QWidget(parent), ui(new Ui::Partners) {
    ui->setupUi(this);
    this->showFullScreen();
    this->setWindowFlag(Qt::FramelessWindowHint);
    scene = new QGraphicsScene;
    ui->MainView->setScene(scene);
    scene->clear();
    QString str;
    str = QFileInfo("Euclase").absolutePath();
    str += "/Materials/Partners.jpg";
    QPixmap Map(str);
    QSize MapSize = ui->MainView->size();
    Map = Map.scaled(MapSize, Qt::KeepAspectRatio);
    scene->addPixmap(Map);
}

Partners::~Partners() {}
