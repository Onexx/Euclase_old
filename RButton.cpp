#include "RButton.h"
#include "Menu.h"

RButton::RButton(QWidget *parent) : QWidget(parent), ui(new Ui::RButton) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    connect(ui->PbReturn, &QPushButton::clicked, this, &RButton::Return);
}

void RButton::Return() {
    menu->show();
    if (!nav->isHidden()) {
        nav->hide();
    }
    if (!info->isHidden()) {
        info->hide();
    }
    if (!prtn->isHidden()) {
        prtn->hide();
    }
    if (!descr->isHidden()) {
        descr->hide();
    }
    if (!tmtbl->isHidden()) {
        tmtbl->hide();
    }
    if (!reg->isHidden()) {
        reg->hide();
    }
    this->hide();
}

RButton::~RButton() { delete ui; }
