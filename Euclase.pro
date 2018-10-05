#-------------------------------------------------
#
# Project created by QtCreator 2017-10-31T14:42:37
#
#-------------------------------------------------

QT += core gui printsupport widgets network
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Euclase
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        DragAndDrop/DropScene.cpp \
        DragAndDrop/DragCircle.cpp \
        DragAndDrop/DragRect.cpp \
        ObservableItems/ObservableRect.cpp \
        BarcodePrinter/BarcodePrinter.cpp \
        ObservableItems/ObservableCircle.cpp \
        Navig/Navig.cpp \
        Menu.cpp \
        RButton.cpp \
        Info/InfoView.cpp \
        Info/Partners.cpp \
        Info/Description.cpp \
        Info/Timetable.cpp \
    RegForm.cpp \
    BarcodePrinter/Printer.cpp

HEADERS += \
        DragAndDrop/DropScene.h \
        DragAndDrop/DragCircle.h \
        DragAndDrop/DragRect.h \
        ObservableItems/ObservableRect.h \
        BarcodePrinter/BarcodePrinter.h \
        ObservableItems/ObservableCircle.h \
        Statistics/Statistics.h \
        Navig/Navig.h \
        Menu.h \
        RButton.h \
        Info/InfoView.h \
        Info/Partners.h \
        Info/Description.h \
        Info/Timetable.h \
        RegForm.h \
        Statistics/Logs.h \
    BarcodePrinter/Printer.h

FORMS += \
        Navig/Navig.ui \
        Menu.ui \
        RButton.ui \
        Info/InfoView.ui \
        Info/Partners.ui \
        Info/Description.ui \
        Info/Timetable.ui \
        RegForm.ui
