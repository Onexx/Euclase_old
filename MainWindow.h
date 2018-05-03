#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QMainWindow>

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
    void block();
    void open();
    void OpenMap();
    void SaveMap();

private:
	Ui::MainWindow *ui;
	QGraphicsScene *scene;
};

class DragCircle: public QGraphicsEllipseItem
{
public:
    explicit DragCircle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};


class DragRect : public QGraphicsRectItem
{
public:
    explicit DragRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

class ObservableRect : public QGraphicsRectItem
{

public:
    explicit ObservableRect(const QRectF &rect, QGraphicsItem *parent = 0);
    enum PointType { None,
    ResizeLeftTop, ResizeTop, ResizeRightTop,
    ResizeLeft, ResizeRight, ResizeLeftBottom,
    ResizeBottom, ResizeRightBottom, Rotate, Move
    };

    QRectF boundingRect() const;

private:

    PointType pointTypeAtPos( const QPointF &pos ) const;
    void setCursorShape( const QPointF &pos );

    qreal w,h;

    PointType pointType;
    QPointF startItemPoint;
    QPointF startMousePoint;
    qreal kx,ky,alpha;
    qreal kx0,ky0,alpha0;
    QTransform transform, transform_inv;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void BCGen(QString number, QString name);
    QPointF previousPosition;

protected:
  void mousePressEvent( QGraphicsSceneMouseEvent *event );
  void mouseMoveEvent ( QGraphicsSceneMouseEvent *event );
  void hoverMoveEvent ( QGraphicsSceneHoverEvent *event );
};

class ObservableCircle : public QGraphicsEllipseItem
{
public:
    enum Direction { Up, Right, Down, Left };
    Direction CurrentDirection;
    explicit ObservableCircle(const QRectF &rect, QGraphicsItem *parent = 0);
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    void BCGen(int number, const QPointF &pos);//generating barcode

    QPointF previousPosition;
};

class DropScene : public QGraphicsScene
{
	Q_OBJECT
public:
    qreal R=153,G=255,B=255;
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private:
    void addCircle(const QPointF &pos);
    void addRect(const QPointF &pos);
};

#endif // MAINWINDOW_H
