#include <QApplication>
#include <QStackedWidget>
#include "Menu.h"
#include "Statistics/logs.h"

int main(int argc, char* argv[]) {
    LogPrint("Oh, it's line of text");
    LogPrint("And this on next line (**)");
    QApplication a(argc, argv);
    Menu m;
    m.showFullScreen();
    return a.exec();
}
