#pragma once
#include <QWidget>
#include "ui_RButton.h"

namespace Ui {
class RButton;
}

class Menu;
class Navig;

class RButton : public QWidget {
    Q_OBJECT

  public:
    explicit RButton(QWidget *parent = 0);
    ~RButton();
    Menu *menu;
    Navig *nav;
    class InfoView *info;
    class Partners *prtn;
    class Description *descr;
    class Timetable *tmtbl;
    class RegForm *reg;

  private slots:
    void Return();

  private:
    Ui::RButton *ui;
};
