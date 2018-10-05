#pragma once

#include <QFileDialog>
#include <QGraphicsScene>
#include <QPen>
#include <QWidget>
#include "ui_Description.h"

namespace Ui {
class Description;
}

class Description : public QWidget {
    Q_OBJECT

  public:
    explicit Description(QWidget *parent = 0);
    ~Description();

  private:
    Ui::Description *ui;
    QGraphicsScene *scene;
};
