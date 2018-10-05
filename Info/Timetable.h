#pragma once

#include <QFileDialog>
#include <QGraphicsScene>
#include <QPen>
#include <QWidget>
#include "ui_Timetable.h"

namespace Ui {
class Timetable;
}

class Timetable : public QWidget {
    Q_OBJECT

  public:
    explicit Timetable(QWidget *parent = 0);
    ~Timetable();

  private:
    Ui::Timetable *ui;
    QGraphicsScene *scene;
};
