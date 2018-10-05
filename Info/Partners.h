#pragma once

#include <QFileDialog>
#include <QGraphicsScene>
#include <QPen>
#include <QWidget>
#include "ui_Partners.h"

namespace Ui {
class Partners;
}

class Partners : public QWidget {
    Q_OBJECT

  public:
    explicit Partners(QWidget *parent = 0);
    ~Partners();

  private:
    Ui::Partners *ui;
    QGraphicsScene *scene;
};
