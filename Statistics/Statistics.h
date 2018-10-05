#pragma once

#include <QDate>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

inline void StatisticsCreate() {
    QString str = "Info/Statistics.txt";
    QFile file(str);
    QTextStream out(&file);
    if (file.open(QIODevice::WriteOnly)) {
        QDate d = QDate::currentDate();
        QString date = d.toString("d.MM.yyyy");
        qDebug() << "Date of stats file - " << date;
        out << date;
        out << "\r\n";
        out.flush();
    } else {
        qDebug() << "Error while opening stats file";
    }
    file.close();
}

inline void StatsPrintInc(QString name) {
    QFile file("Info/Statistics.txt");
    QTextStream out(&file);
    if (file.open(QIODevice::Text | QIODevice::ReadWrite)) {
        {
            QString tname = name;
            name.clear();
            for (int i = 0; i < tname.size(); i++) {
                if (tname[i] != '\n' && tname[i] != '\r') {
                    name += tname[i];
                }
            }
            qDebug() << name << " - change completed";
        }
        name += "\r\n";
        while (!file.atEnd()) {
            QString s = file.readLine();
            QStringList lst = s.split(" ");
            if (s == name) {
                s = file.readLine();
                s = file.readLine();
                lst = s.split(" ");
                int kol;
                qDebug() << s;
                kol = lst.at(2).toInt() + 1;
                qDebug() << kol;
                QString kols;
                kols = QString::number(kol);
                file.seek(file.pos() - s.length() - 1);
                s.clear();
                s.insert(0, lst.at(0));
                s.insert(s.size(), " ");
                s.insert(s.size(), lst.at(1));
                s.insert(s.size(), " ");
                s.insert(s.size(), kols);
                s.insert(s.size(), " \r\n");
                out << s;
                break;
            }
        }
    } else {
        qDebug() << "Can't open file";
    }
    file.close();
}

inline void StatsClickInc(QString name) {
    QFile file("Info/Statistics.txt");
    QTextStream out(&file);
    if (file.open(QIODevice::Text | QIODevice::ReadWrite)) {
        {
            QString tname = name;
            name.clear();
            for (int i = 0; i < tname.size(); i++) {
                if (tname[i] != '\n' && tname[i] != '\r') {
                    name += tname[i];
                }
            }
            qDebug() << name << " - change completed";
        }
        name += "\r\n";
        bool key = false;
        while (!file.atEnd()) {
            QString s = file.readLine();
            QStringList lst = s.split(" ");
            if (s == name) {
                key = true;
                s = file.readLine();
                lst = s.split(" ");
                int kol;
                qDebug() << s;
                kol = lst.at(2).toInt() + 1;
                qDebug() << kol;
                QString kols;
                kols = QString::number(kol);
                file.seek(file.pos() - s.length() - 1);
                s.clear();
                s.insert(0, lst.at(0));
                s.insert(s.size(), " ");
                s.insert(s.size(), lst.at(1));
                s.insert(s.size(), " ");
                s.insert(s.size(), kols);
                s.insert(s.size(), " \r\n");
                out << s;
                break;
            }
        }
        if (!key) {
            out << name << "Clicked - 1 \r\nPrinted - 0 \r\n\r\n";
            qDebug() << "Added new profile - " << name << "\r\n";
        }
    } else {
        qDebug() << "Can't open file";
    }
    file.close();
}
