#pragma once

#include <QDate>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

inline void LogPrint(QString text) {
    QDateTime DT = QDateTime::currentDateTime();
    QFile Log("Info/Logs.Log");
    if (Log.open(QIODevice::Append | QFile::Text)) {
        QTextStream out(&Log);
        out << DT.time().toString("H:mm:ss") << " - ";
        out << text;
        out << '\n';
        Log.close();
    }
}
