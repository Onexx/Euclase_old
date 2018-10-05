#include "Menu.h"

Menu::Menu(QWidget *parent) : QMainWindow(parent), ui(new Ui::Menu) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->PbNavig, &QPushButton::clicked, this, &Menu::Navig);
    connect(ui->PbInfo, &QPushButton::clicked, this, &Menu::Info);
    connect(ui->PbPartners, &QPushButton::clicked, this, &Menu::Partners);
    connect(ui->PbHeader, &QPushButton::clicked, this, &Menu::Description);
    connect(ui->PbTimetable, &QPushButton::clicked, this, &Menu::Timetable);
    connect(ui->PbCamera, &QPushButton::clicked, this, &Menu::Hide);
    connect(ui->PbReg, &QPushButton::clicked, this, &Menu::Register);
    nav = new class Navig();
    reg = new class RegForm();
    info = new class InfoView();
    prtn = new class Partners();
    descr = new class Description();
    tmtbl = new class Timetable();
    rb = new class RButton();
    rb->setGeometry((this->size().width() - rb->size().width() - 10), 10, 60,
                    60);
    rb->move((info->size().width() - rb->size().width() - 10), 10);
    rb->menu = this;
    rb->nav = nav;
    rb->info = info;
    rb->prtn = prtn;
    rb->descr = descr;
    rb->tmtbl = tmtbl;
    rb->reg = reg;
}

void Menu::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        this->close();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void Menu::Navig() {
    rb->show();
    nav->show();
    this->hide();

    auto scene = new QGraphicsScene;
    nav->ui->GvNames->setScene(scene);
    scene->clear();
    QString str;
    str = QFileInfo("Euclase").absolutePath();
    str += "/Materials/Names.jpg";
    LogPrint(str);
    QPixmap Map3(str);
    qDebug() << this->size().height();
    QSize MapSize(400, this->size().height());
    qDebug() << MapSize.width() << ' ' << MapSize.height();
    Map3 = Map3.scaled(MapSize, Qt::KeepAspectRatio);
    scene->addPixmap(Map3);
}

void Menu::Info() {
    rb->show();
    info->show();
    this->hide();
}

void Menu::Partners() {
    rb->show();
    prtn->show();
    this->hide();
}

void Menu::Description() {
    rb->show();
    descr->show();
    this->hide();
}

void Menu::Timetable() {
    rb->show();
    tmtbl->show();
    this->hide();
}

void Menu::Hide() {
    rb->show();
    this->hide();
}

void Menu::Register() {
    rb->show();
    reg->showFullScreen();
    this->hide();
}

Menu::~Menu() {
    // Deleting menu
    delete ui;
    // Deleting navigation
    QFile Tfile("FSave.Etnm");
    Tfile.remove();
    delete nav->Navig::ui;
}
