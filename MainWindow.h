#pragma once

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QMainWindow>
#include <QPen>
#include "DragAndDrop/DropScene.h"
#include "DragAndDrop/DragCircle.h"
#include "DragAndDrop/DragRect.h"
#include "ObservableItems/ObservableRect.h"
#include "ObservableItems/ObservableCircle.h"
#include "ui_MainWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void Block();
    void Open();
    void OpenMap();
    void SaveMap();

private:
	Ui::MainWindow *ui;
	QGraphicsScene *scene;
};
