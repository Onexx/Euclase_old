#pragma once
#include <QMainWindow>
#include "Info/Description.h"
#include "Info/InfoView.h"
#include "Info/Partners.h"
#include "Info/Timetable.h"
#include "Navig/Navig.h"
#include "RButton.h"
#include "RegForm.h"
#include "ui_Menu.h"

namespace Ui {
class Menu;
}

class Menu : public QMainWindow {
    Q_OBJECT

  public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

  private slots:
    void Navig();
    void Info();
    void Partners();
    void Description();
    void Hide();
    void Timetable();
    void Register();

  private:
    Ui::Menu *ui;
    class Navig *nav;
    class InfoView *info;
    class RButton *rb;
    class Description *descr;
    class Partners *prtn;
    class Timetable *tmtbl;
    class RegForm *reg;
};
