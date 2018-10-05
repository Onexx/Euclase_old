#pragma once

#include <QFile>
#include <QWidget>
#include <QtCore/QWeakPointer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include "ui_RegForm.h"
#include "BarcodePrinter/Printer.h"

namespace Ui {
class RegForm;
}

class RegForm : public QWidget {
    Q_OBJECT

  public:
    explicit RegForm(QWidget *parent = nullptr);
    ~RegForm();

  private:
    void onIdRequest();
    void onRegRequest();

  private:
    Ui::RegForm *ui;
    QScopedPointer<QTcpSocket> mCurrentSocket;
    QHostAddress mHostAddress;
};
