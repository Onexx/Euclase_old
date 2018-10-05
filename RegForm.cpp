#include "RegForm.h"
#include "Statistics/logs.h"

RegForm::RegForm(QWidget *parent) : QWidget(parent), ui(new Ui::RegForm) {
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    QFile file("Settings/Host.txt");
    if (file.open(QIODevice::ReadWrite)) {
        const QString ip = QString::fromUtf8(file.readLine());
        mHostAddress.setAddress(ip);
    }
    connect(ui->requestButton, &QPushButton::pressed, this,
            &RegForm::onIdRequest);
    connect(ui->registerButton, &QPushButton::pressed, this,
            &RegForm::onRegRequest);
}

void RegForm::onIdRequest() {
    const QString id = ui->lineEditId->text();
    if (id.isEmpty()) {
        return;
    }
    mCurrentSocket.reset(new QTcpSocket());
    mCurrentSocket->connectToHost(mHostAddress, 9999);
    mCurrentSocket->waitForConnected();
    QString request = QString("id %1").arg(id);
    mCurrentSocket->write(request.toLocal8Bit(), request.size());
    mCurrentSocket->waitForBytesWritten();
    mCurrentSocket->waitForReadyRead();
    QString localTrimmed =
        QString::fromLocal8Bit(mCurrentSocket->readAll()).trimmed();
    if (localTrimmed.contains(id)) {
        QString name =
            localTrimmed.split(QRegExp("[\r\n]"), QString::SkipEmptyParts)[0]
                .section("name: ", 1);
        ui->lineEditName->setText(name);
    }
}

void RegForm::onRegRequest() {
    if (mCurrentSocket->isOpen()) {
        mCurrentSocket->write("Y", 1);
        Printer* printer = new Printer();
        QString PName;
        QFile file("Settings/PrinterName.txt");
        if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
            PName = file.readLine();
        } else {
            qDebug() << "Printer name not found";
            LogPrint("Printer name not found");
        }
        LogPrint(PName);
        printer->configurePrinter(PName);
        LogPrint("1");
        QString id = ui->lineEditId->text(),name = ui->lineEditName->text();
        LogPrint(ui->lineEditId->text());
        LogPrint(ui->lineEditName->text());
        LogPrint(id);
        LogPrint(name);
        printer->Print(ui->lineEditId->text(), ui->lineEditName->text());
        mCurrentSocket->waitForBytesWritten();
    }
}

RegForm::~RegForm() {}
