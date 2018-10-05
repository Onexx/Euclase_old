#pragma once

#include <QObject>
#include <QtPrintSupport>

#define CODE128_B_START 104
#define CODE128_STOP 106

class Printer : public QObject {
    Q_OBJECT
  public:
    explicit Printer(QObject* parent = nullptr);

    /** Lists all available printers */
    QStringList getAvailablePrinters();
    /** Configures the printer, if none selected defaults to pdf printer */
    bool configurePrinter(QString printerName = QString());
    /** Prints the text as a barcode */
    void Print(QString id, QString name);

  private:
    /** Configures the page */
    void configurePage();

    /** Adds start/check/stop characters to the code */
    QString encodeBarcode(QString code);

    /** Calculates the checksum character from the code */
    int calculateCheckCharacter(QString code);

    int codeToChar(int code);
    int charToCode(int ch);

    bool printerConfigured;
    QPrinter mPrinter;
};
