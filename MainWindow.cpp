#include "MainWindow.h"
#include "Qt-barcode-master/barcodeprinter.h"
#include "ui_MainWindow.h"
#include <QtWidgets>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QDrag>
#include <QDate>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QVariant>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QFileDialog>
#include <QBrush>
#include <QMessageBox>
#include <QAbstractScrollArea>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    connect(ui->PbBlock, &QPushButton::clicked, this, &MainWindow::block);
    connect(ui->PbOpen, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->PbOpenMap, &QPushButton::clicked, this, &MainWindow::OpenMap);
    connect(ui->PbSaveMap, &QPushButton::clicked, this, &MainWindow::SaveMap);
	scene = new DropScene();
	ui->graphicsView->setAcceptDrops(true);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->setAttribute(Qt::WA_AcceptTouchEvents);
	ui->graphicsView_2->setAttribute(Qt::WA_AcceptTouchEvents);
    auto scene2 = new QGraphicsScene();
	ui->graphicsView_2->setScene(scene2);
	auto c = new DragCircle(0, 0, 100, 100);
	auto r = new DragRect(0, 150, 100, 100);
	scene2->addItem(c);
	scene2->addItem(r);
    QString str = "Info/Statistics.txt";
    QFile file(str);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly)){
        QDate d = QDate::currentDate();
        QString date = d.toString("d.MM.yyyy");
        qDebug()<<"Date of stats file - "<<date;
        out<<date;
        out<<"\r\n";
        out.flush();
    }else{
        qDebug()<<"Error while opening stats file";
    }
    file.close();
}

MainWindow::~MainWindow()
{
    QString Tstr = "FSave.Etnm";
    QFile Tfile(Tstr);
    Tfile.remove();
    delete ui;
}

void MainWindow::open()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.png *.jpg *.jpeg *.bmp *pdf");
    scene->clear();
    QPixmap Map(str);
    QSize MapSize = ui->graphicsView->size();
    Map = Map.scaled(MapSize,Qt::KeepAspectRatio);
    scene->addPixmap(Map);
    Map = Map.scaled(MapSize,Qt::KeepAspectRatio);
    QFile file("FSave.Etnm");
    if(!(file.open(QIODevice::WriteOnly))){
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_2);
    out<<Map;
    file.close();
}

void MainWindow::SaveMap(){
    QString Tstr = "FSave.Etnm";
    QFile Tfile(Tstr);
    if(!(Tfile.open(QIODevice::ReadOnly))){
        return;
    }
    QDataStream in(&Tfile);
    in.setVersion(QDataStream::Qt_4_2);
    QPixmap Map;
    in>>Map;
    Tfile.close();
    Tfile.remove();
    QString SaveName = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("Euclase Navigation Map(*.Enm)"));
    QSize MapSize = ui->graphicsView->size();
    Map = Map.scaled(MapSize,Qt::KeepAspectRatio);
    QFile file(SaveName);
    if(!(file.open(QIODevice::WriteOnly))){
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_2);

    int kol = 0;
    foreach(QGraphicsItem *item, ui->graphicsView->scene()->items()){
        if(item->type()==QGraphicsEllipseItem::Type||item->type()==QGraphicsRectItem::Type){
            kol++;
        }
    }
    out<<Map;
    qDebug()<<"kol = "<<kol;
    out<<kol;
    foreach(QGraphicsItem *item, ui->graphicsView->scene()->items()){
        if(item->type()==QGraphicsEllipseItem::Type){
            QString Type="ellipse";
            out<<Type;
            int idx=item->data(Qt::UserRole).toInt(),w = item->boundingRect().width()-1, h = item->boundingRect().height()-1;
            QPointF pos = item->pos();
            out<<w<<h;
            out<<pos;
            out<<idx;
            qDebug()<<w<<' '<<h<<' '<<pos<<' '<<idx<<endl;
        }
        if(item->type()==QGraphicsRectItem::Type){
            QString Type="rect";
            out<<Type;
            int idx=item->data(Qt::UserRole).toInt(),w = item->boundingRect().width()-2, h = item->boundingRect().height()-2;
            QPointF pos = item->pos();
            out<<w<<h;
            out<<pos;
            out<<idx;
            qDebug()<<w<<' '<<h<<' '<<pos<<' '<<idx<<endl;
        }
    }
    file.close();
}

void MainWindow::OpenMap(){
    qDebug()<<"Opening the map";
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.Enm");
    QFile file(str);
    if(!(file.open(QIODevice::ReadOnly))){
        qDebug()<<"Error while opening map";
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_2);
    scene->clear();
    QPixmap Map;
    in>>Map;
    scene->addPixmap(Map);
    int kol;
    in>>kol;
    int R=100,G=100,B=100;
    bool key2 = true;
    QFile Cfile("Settings/Conf.txt");
    if(Cfile.open(QIODevice::Text | QIODevice::ReadOnly)){
        while(!Cfile.atEnd()){
            bool key = true;
            key2=false;
            QString s = Cfile.readLine();
            QStringList lst = s.split(" ");
            if(lst.at(0) == "Colour" && lst.at(1) == "="){
                R=lst.at(2).toInt(&key);
                G=lst.at(3).toInt(&key);
                B=lst.at(4).toInt(&key);
                key=false;
            }else{
                if(lst.at(0) == "HeightOfCircle" || lst.at(0) == "WidthOfCircle" || lst.at(0) == "HeightOfRectangle" || lst.at(0) == "WidthOfRectangle" || (s[0] == '/' && s[1] == '/')){
                    key=false;
                }
            }
            if(key){
                QMessageBox(QMessageBox::Information, "Conf.txt", "File written encorrectly, reutrning to standart configurations",
                QMessageBox::Ok);
                Cfile.close();
                if(Cfile.open(QIODevice::Text | QIODevice::WriteOnly)){
                    Cfile.write("//You can change width or height walues here, but don't forget spaces\r\n "
                               "WidthOfRectangle = 50\r\n"
                               "HeightOfRectangle = 40\r\n"
                               "WidthOfCircle = 30\r\n"
                               "HeightOfCircle = 30\r\n"
                               "//Change colour walue in format R G B\r\n"
                               "Colour = 153 255 255\r\n");
                }
                qDebug()<<"Rewriting the configurations";
                break;
            }
        }
    }else{
        qDebug()<<"Can't open configuratons file";
    }
    if(key2){
        Cfile.close();
        if(Cfile.open(QIODevice::Text | QIODevice::WriteOnly)){
            Cfile.write("//You can change width or height walues here, but don't forget spaces\r\n"
                       "WidthOfRectangle = 50\r\n"
                       "HeightOfRectangle = 40\r\n"
                       "WidthOfCircle = 30\r\n"
                       "HeightOfCircle = 30\r\n"
                       "//Change colour walue in format R G B\r\n"
                       "Colour = 153 255 255\r\n");
        }
        qDebug()<<"Rewriting the configurations";
    }
    Cfile.close();
    qDebug()<<"Load";
    qDebug() << kol;
    for(int i = 0;i < kol; i++){
        QString Type;
        in>>Type;
        qDebug()<<"Type = "<<Type;
        if(Type == "ellipse"){
            QPointF pos;
            int w, h, idx;
            in>>w>>h;
            in>>pos;
            in>>idx;
            qDebug() << w << h << pos << idx;
            QSizeF size(w, h);
            auto item = new ObservableCircle(QRectF(QPointF(), size));
            item->setData(Qt::UserRole, idx);
            auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
            number->setFlag(QGraphicsItem::ItemIsSelectable, false);
            number->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
            number->setPos((item->rect().width() - number->boundingRect().width())/2, (item->rect().height() - number->boundingRect().height())/2);
            item->setPos(pos.x(), pos.y());
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setBrush(QBrush(QColor(R, G, B, 150)));//colour
            scene->addItem(item);
        }
        if(Type == "rect"){
            QPointF pos;
            int w,h,idx;
            in>>w>>h;
            in>>pos;
            in>>idx;
            qDebug()<<w<<' '<<h<<' '<<pos<<' '<<idx<<endl;
            QSizeF size(w, h);
            auto item = new ObservableRect(QRectF(QPointF(), size));
            item->setData(Qt::UserRole, idx);
            auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
            number->setFlag(QGraphicsItem::ItemIsSelectable, false);
            number->setPos((item->rect().width() - number->boundingRect().width())/2, (item->rect().height() - number->boundingRect().height())/2);
            item->setPos(pos.x(), pos.y());
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setBrush(QBrush(QColor(R, G, B, 150)));
            scene->addItem(item);
        }
    }

    file.close();
    QSize MapSize = ui->graphicsView->size();
    Map = Map.scaled(MapSize,Qt::KeepAspectRatio);
    QFile fileS("FSave.Etnm");
    if(!(fileS.open(QIODevice::WriteOnly))){
        return;
    }
    QDataStream out(&fileS);
    out.setVersion(QDataStream::Qt_4_2);
    out<<Map;
    foreach(QGraphicsItem *item, ui->graphicsView->scene()->items()){
        if(item->type()==QGraphicsEllipseItem::Type||item->type()==QGraphicsRectItem::Type){
            kol++;
        }
    }
    out<<kol;
    foreach(QGraphicsItem *item, ui->graphicsView->scene()->items()){
        if(item->type()==QGraphicsEllipseItem::Type){
            QString Type="ellipse";
            out<<Type;
            int w = item->boundingRect().width(), h = item->boundingRect().height();
            out<<w<<h;
            out<<item->pos();
            out<<item->data(Qt::UserRole).toInt();
        }
        if(item->type()==QGraphicsRectItem::Type){
            QString Type="rect";
            out<<Type;
            int w = item->boundingRect().width(), h = item->boundingRect().height();
            out<<w<<h;
            out<<item->pos();
            out<<item->data(Qt::UserRole).toInt();
        }
    }
    fileS.close();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        ui->PbBlock->show();
        ui->PbOpen->show();
        ui->PbOpenMap->show();
        ui->PbSaveMap->show();
        ui->graphicsView_2->show();
        int kol=0;
        for (auto item : ui->graphicsView->scene()->items()) {
            if(dynamic_cast<QGraphicsEllipseItem*>(item) || dynamic_cast<QGraphicsRectItem*>(item)){
                item->setFlag(QGraphicsItem::ItemIsMovable, true);
                qDebug()<<kol++;
                item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            }
        }
    }else{
        if(event->key() == Qt::Key_S){
            if(event->modifiers() & Qt::CTRL){
                /*QFile file("Save/FSave.dat");
                if(!(file.open(QIODevice::WriteOnly))){
                    qDebug()<<"Failed to save";
                    return;
                }
                QDataStream out(&file);
                foreach(QGraphicsItem *item, ui->graphicsView->scene()->items()){
                    if(item->type()==QGraphicsEllipseItem::Type){
                        QGraphicsEllipseItem *ellipse=qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
                        out<<ellipse;
                    }
                    if(item->type()==QGraphicsRectItem::Type){
                        QGraphicsRectItem *rect=qgraphicsitem_cast<QGraphicsRectItem*>(item);
                        out<<rect;
                    }
                }
                file.close();
                */
                qDebug()<<"Save";
            }
        }else{
            QWidget::keyPressEvent(event);
        }
    }
}

void MainWindow::block()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, "Blocking", "Editing will be available by pressing Esc button",
    QMessageBox::Ok | QMessageBox::Cancel);
    int n = msgBox->exec();
    delete msgBox;
    if (n == QMessageBox::Ok){
        ui->PbBlock->hide();
        ui->PbOpen->hide();
        ui->PbOpenMap->hide();
        ui->PbSaveMap->hide();
        ui->graphicsView_2->hide();
        for (auto item : ui->graphicsView->scene()->items()) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        }
    }
}
DragCircle::DragCircle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
	: QGraphicsEllipseItem(x, y, w, h, parent)
{
}

void DragCircle::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	QDrag *drag = new QDrag(this->scene());
	QMimeData *data = new QMimeData();
    data->setText("circle");
	drag->setMimeData(data);
	drag->start();
}

DragRect::DragRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
	: QGraphicsRectItem(x, y, w, h, parent)
{
}

void DragRect::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	QDrag *drag = new QDrag(this->scene());
	QMimeData *data = new QMimeData();
    data->setText("rect");
	drag->setMimeData(data);
	drag->start();
}

void DropScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
	} else {
		QGraphicsScene::dragEnterEvent(event);
	}
}

void DropScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
	} else {
		QGraphicsScene::dragMoveEvent(event);
	}
}

void DropScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
	} else {
		QGraphicsScene::dragLeaveEvent(event);
	}
}

void DropScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
		if (event->mimeData()->text() == "circle") {
			addCircle(event->scenePos());
		} else {
			addRect(event->scenePos());
		}
	} else {
		QGraphicsScene::dropEvent(event);
    }
}

void DropScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete) {
		QList<QGraphicsItem *> items = selectedItems();
		for (auto item : items) {
			delete item;
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void DropScene::addCircle(const QPointF &pos)
{
	int idx = 1;
	while (true) {
        bool exists = false;
		for (auto i : items()) {
			if (dynamic_cast<QGraphicsEllipseItem*>(i)) {
				if (i->data(Qt::UserRole).toInt() == idx) {
					exists = true;
					break;
				}
			}
		}
		if (exists) ++idx; else break;
    }
    auto Confw=0, Confh=0;
    bool key2=true;
    QFile file("Settings/Conf.txt");
    if(file.open(QIODevice::Text | QIODevice::ReadOnly)){
        while(!file.atEnd()){
            key2=false;
            bool key = true;
            QString s = file.readLine();
            QStringList lst = s.split(" ");
            if(lst.at(0) == "WidthOfCircle" && lst.at(1) == "="){
                Confw=lst.at(2).toInt(&key);
                key=false;
            }else{
                if(lst.at(0) == "HeightOfCircle" && lst.at(1) == "="){
                    Confh=lst.at(2).toInt(&key);
                    key=false;
                }else{
                    if(lst.at(0) == "Colour" && lst.at(1) == "="){
                        R=lst.at(2).toInt(&key);
                        G=lst.at(3).toInt(&key);
                        B=lst.at(4).toInt(&key);
                        key=false;
                    }else{
                        if(lst.at(0) == "HeightOfRectangle" || lst.at(0) == "WidthOfRectangle" || (s[0] == '/' && s[1] == '/')){
                            key=false;
                        }
                    }
                }
            }
            if(key){
                QMessageBox(QMessageBox::Information, "Conf.txt", "File written encorrectly, reutrning to standart configurations",
                QMessageBox::Ok);
                file.close();
                if(file.open(QIODevice::Text | QIODevice::WriteOnly)){
                    file.write("//You can change width or height walues here, but don't forget spaces\r\n"
                               "WidthOfRectangle = 50\r\n"
                               "HeightOfRectangle = 40\r\n"
                               "WidthOfCircle = 30\r\n"
                               "HeightOfCircle = 30\r\n"
                               "//Change colour walue in format R G B\r\n"
                               "Colour = 153 255 255\r\n");
                }
                Confh=30;
                Confw=30;
                qDebug()<<"Rewriting the configurations";
                break;
            }
        }
    }else{
        qDebug()<<"Can't open configurations file";
        Confh=30;
        Confw=30;
    }
    if(key2){
        qDebug()<<"Rewriting the configurations:Empty file";
        file.close();
        if(file.open(QIODevice::Text | QIODevice::WriteOnly)){
            file.write("//You can change width or height walues here, but don't forget spaces\r\n"
                       "WidthOfRectangle = 50\r\n"
                       "HeightOfRectangle = 40\r\n"
                       "WidthOfCircle = 30\r\n"
                       "HeightOfCircle = 30\r\n"
                       "//Change colour walue in format R G B\r\n"
                       "Colour = 153 255 255\r\n");
        }
        Confh=30;
        Confw=30;
    }
    file.close();
    QSizeF size(Confw, Confh);
    auto item = new ObservableCircle(QRectF(QPointF(), size));
	auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
	number->setFlag(QGraphicsItem::ItemIsSelectable, false);
    number->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    number->setPos((item->rect().width() - number->boundingRect().width())/2, (item->rect().height() - number->boundingRect().height())/2);
	item->setPos(pos.x() - size.width()/2, pos.y() - size.height()/2);
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setBrush(QBrush(QColor(R, G, B, 150)));//colour
	addItem(item);
}

void DropScene::addRect(const QPointF &pos)
{
	int idx = 1;
	while (true) {
		bool exists = false;
		for (auto i : items()) {
			if (dynamic_cast<QGraphicsRectItem*>(i)) {
				if (i->data(Qt::UserRole).toInt() == idx) {
					exists = true;
					break;
				}
			}
		}
		if (exists) ++idx; else break;
	}
    auto Confw=0, Confh=0;
    bool key2=true;
    QFile file("Settings/Conf.txt");
    if(file.open(QIODevice::Text | QIODevice::ReadOnly)){
        while(!file.atEnd()){
            key2=false;
            bool key=true;
            QString s = file.readLine();
            QStringList lst = s.split(" ");
            if(lst.at(0) == "WidthOfRectangle" && lst.at(1) == "="){
                Confw=lst.at(2).toInt(&key);
                key=false;
            }else{
                if(lst.at(0) == "HeightOfRectangle" && lst.at(1) == "="){
                    Confh=lst.at(2).toInt(&key);
                    key=false;
                }else{
                    if(lst.at(0) == "Colour" && lst.at(1) == "="){
                        R=lst.at(2).toInt(&key);
                        G=lst.at(3).toInt(&key);
                        B=lst.at(4).toInt(&key);
                        key=false;
                    }else{
                        if(lst.at(0) == "HeightOfCircle" || lst.at(0) == "WidthOfCircle" || (s[0] == '/' && s[1] == '/')){
                            key=false;
                        }
                    }
                }
            }
            if(key){
                //qDebug()<<s;
                file.close();
                if(file.open(QIODevice::Text | QIODevice::WriteOnly)){
                    file.write("//You can change width or height walues here, but don't forget spaces\r\n"
                               "WidthOfRectangle = 50\r\n"
                               "HeightOfRectangle = 40\r\n"
                               "WidthOfCircle = 30\r\n"
                               "HeightOfCircle = 30\r\n"
                               "//Change colour walue in format R G B\r\n"
                               "Colour = 153 255 255\r\n");
                }
                Confh=50;
                Confw=50;
                qDebug()<<"Rewriting the configurations";
                break;
            }
        }
    }else{
        qDebug()<<"Can't open configurations file";
        Confh=50;
        Confw=50;
    }
    if(key2){
        qDebug()<<"Rewriting the configurations:Empty file";
        file.close();
        if(file.open(QIODevice::Text | QIODevice::WriteOnly)){
            file.write("//You can change width or height walues here, but don't forget spaces\r\n"
                       "WidthOfRectangle = 50\r\n"
                       "HeightOfRectangle = 40\r\n"
                       "WidthOfCircle = 30\r\n"
                       "HeightOfCircle = 30\r\n"
                       "//Change colour walue in format R G B\r\n"
                       "Colour = 153 255 255\r\n");
        }
        Confh=50;
        Confw=50;
    }
    file.close();
    QSizeF size(Confw, Confh);
	auto item = new ObservableRect(QRectF(QPointF(), size));
	item->setData(Qt::UserRole, idx);
	auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
	number->setFlag(QGraphicsItem::ItemIsSelectable, false);
    number->setPos((item->rect().width() - number->boundingRect().width())/2, (item->rect().height() - number->boundingRect().height())/2);
	item->setPos(pos.x() - size.width()/2, pos.y() - size.height()/2);
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setBrush(QBrush(QColor(R, G, B, 150)));
	addItem(item);
}

ObservableRect::ObservableRect( const QRectF &rect, QGraphicsItem * parent )
  : QGraphicsRectItem( rect, parent )
{
  w = rect.width();
  h = rect.height();
  pointType = None;
  kx = ky = 1.0;
  alpha = 0.0;

  setAcceptHoverEvents ( true );
 }

QRectF ObservableRect::boundingRect() const
{
  const qreal border = 1;
  return QRectF( -border, -border, w+2*border, h+2*border );
}

void ObservableRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
	event->accept();
	previousPosition = event->scenePos();
    if( isSelected() ){
        if(event->button() == Qt::LeftButton)
        {
          startMousePoint      = event->scenePos();
          startItemPoint       = scenePos();
          pointType            = pointTypeAtPos( event->pos() );
          setCursorShape( event->pos() );
          kx0 = kx;
          ky0 = ky;
          alpha0 = alpha;

          QPointF p0;
          switch( pointType )
          { case( ResizeLeftTop ):     p0 = QPointF(w,  h ); break;
            case( ResizeTop ):         p0 = QPointF(w/2,  h ); break;
            case( ResizeRightTop ):    p0 = QPointF(0,  h ); break;
            case( ResizeLeft ):        p0 = QPointF(w, h/2); break;
            case( ResizeRight ):       p0 = QPointF(0, h/2 ); break;
            case( ResizeLeftBottom ):  p0 = QPointF(w, 0 ); break;
            case( ResizeBottom ):      p0 = QPointF(w/2, 0 ); break;
            case( ResizeRightBottom ): p0 = QPointF(0, 0 ); break;
            default:;
          }
          p0 = mapToScene( p0 );

          transform.reset();
          transform.rotateRadians( -alpha0 );
          transform.translate( -p0.x(), -p0.y() );
          transform_inv = transform.inverted(0);
        }
    }
}

void ObservableRect::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsItem::mouseMoveEvent( event );

  if( !isSelected() ) return;

  if( pointType == None ) return;

  if( pointType == Move )
  {
    QPointF p = startItemPoint + event->scenePos() - startMousePoint;
    setPos( mapToParent( mapFromScene( p ) ) );
    return;
  }

  if( pointType != Rotate )
  {
    QPointF pm = transform.map( startMousePoint );
    QPointF pi = transform.map( startItemPoint );
    QPointF pp = transform.map( event->scenePos() );

    qreal kxx = pp.x() / pm.x();
    qreal kyy = pp.y() / pm.y();

    switch( pointType )
    { case( ResizeTop ):
      case( ResizeBottom ):
        kxx = 1;
        break;
      case( ResizeLeft ):
      case( ResizeRight ):
        kyy = 1;
        break;
      default:;
    }

    kx = kx0 * kxx;
    ky = ky0 * kyy;

    qreal xx = pi.x() * kxx;
    qreal yy = pi.y() * kyy;

    QTransform tr;
    tr.rotateRadians( alpha0 );
    tr.scale( kx,ky );

    setTransform( tr );
    setPos( mapToParent( mapFromScene( transform_inv.map( QPointF(xx,yy) ) ) ) );
    return;
  }
  if( pointType == Rotate )
  {
    QPointF p1 = transform.map( startMousePoint );
    QPointF p2 = transform.map( event->scenePos() );

    alpha = alpha0 + atan2( p2.y(), p2.x() ) - atan2( p1.y(), p1.x() );

    QTransform tr;
    tr.rotateRadians( alpha );
    tr.scale( kx,ky );
    setTransform( tr );
    return;
  }
}

void ObservableRect::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
{
  if(isSelected())
  {
    setCursorShape( event->pos() );
  } else
  { setCursor(Qt::ArrowCursor);
  }
}

void ObservableRect::setCursorShape( const QPointF &pos )
{
  switch( pointTypeAtPos( pos ) )
  {
   case( ResizeLeftTop ):     setCursor(Qt::SizeFDiagCursor); break;
   case( ResizeTop ):         setCursor(Qt::SizeVerCursor);   break;
   case( ResizeRightTop ):    setCursor(Qt::SizeBDiagCursor); break;
   case( ResizeLeft ):        setCursor(Qt::SizeHorCursor);   break;
   case( ResizeRight ):       setCursor(Qt::SizeHorCursor);   break;
   case( ResizeLeftBottom ):  setCursor(Qt::SizeBDiagCursor); break;
   case( ResizeBottom ):      setCursor(Qt::SizeVerCursor);   break;
   case( ResizeRightBottom ): setCursor(Qt::SizeFDiagCursor); break;
   case( Rotate ):            setCursor(Qt::SizeAllCursor);   break;
   case( Move ):              setCursor(Qt::OpenHandCursor);  break;
   default:
    setCursor(Qt::ArrowCursor);
  }
}

inline static qreal norm( qreal x, qreal y )
{
  return x*x + y*y;
}

ObservableRect::PointType ObservableRect::pointTypeAtPos(  const QPointF &pos  ) const
{
  qreal x = pos.x();
  qreal y = pos.y();
  qreal r = 100;
  qDebug() << x << y;

  if( norm(x, y ) < r) return ResizeLeftTop;
  if( norm(x-w, y ) < r) return ResizeRightTop;
  if( norm(x,  y-h ) < r) return ResizeLeftBottom;
  if( norm(x-w,  y-h ) < r) return ResizeRightBottom;

  if(norm(x, y-h/2) < r) return ResizeLeft;
  if(norm(x-w, y-h/2) < r) return ResizeRight;
  if(norm(x-w/2, y) < r) return ResizeTop;
  if(norm(x-w/2, y-h) < r) return ResizeBottom;

  return Move;
}

void ObservableRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	event->accept();
	if (event->scenePos() == previousPosition && !(flags() & QGraphicsItem::ItemIsMovable)) {
        QString name = data(Qt::UserRole).toString();
        QString description="No description";
        QString FileName = "Settings/";
        FileName += data(Qt::UserRole).toString();
        FileName+=".txt";
        QString description2="";
        QFile file(FileName);
        qDebug()<<name;
        qDebug()<<FileName;
        bool key=false;
        if(file.open(QIODevice::Text | QIODevice::ReadOnly)){
            while(!file.atEnd()){
                QString s = file.readLine();
                QStringList lst = s.split(" ");
                if(lst.at(0) == "name" && lst.at(1) == "="){
                    name.clear();
                    for(int i = 7; i < s.size(); i++){
                        name += s[i];
                    }
                }else{
                    description2+=s;
                    key=true;
                }
            }
        }else{
            qDebug()<<"Can't open file";
        }
        if(key){
            description = description2;
        }
        file.close();
        {
        QFile file("Info/Statistics.txt");
        QTextStream out(&file);
        if(file.open(QIODevice::Text | QIODevice::ReadWrite)){
            {
                QString tname = name;
                name.clear();
                for(int i = 0;i < tname.size();i++){
                    if(tname[i] != '\n' && tname[i] != '\r'){
                        name+=tname[i];
                    }
                }
                qDebug()<<name<<" - change completed";
            }
            name+="\r\n";
            bool key=false;
            while(!file.atEnd()){
                QString s = file.readLine();
                QStringList lst = s.split(" ");
                if(s == name){
                    key=true;
                    s = file.readLine();
                    lst = s.split(" ");
                    int kol;
                    qDebug()<<s;
                    kol = lst.at(2).toInt() + 1;
                    qDebug()<<kol;
                    QString kols;
                    kols = QString::number(kol);
                    file.seek(file.pos() - s.length()-1);
                    s.clear();
                    s.insert(0, lst.at(0));
                    s.insert(s.size()," ");
                    s.insert(s.size(), lst.at(1));
                    s.insert(s.size()," ");
                    s.insert(s.size(), kols);
                    s.insert(s.size(), " \r\n");
                    out << s;
                    break;
                }
            }
            if(!key){
                out << name << "Clicked - 1 \r\nPrinted - 0 \r\n\r\n";
                qDebug()<<"Added new profile - "<<name<<"\r\n";
            }
        }else{
            qDebug()<<"Can't open file";
        }
        file.close();
        }
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, name, description,
        QMessageBox::Ok | QMessageBox::Cancel);
        msgBox->setButtonText(QMessageBox::Ok, "Print");
        int n = msgBox->exec();
        if (n == QMessageBox::Ok){
            QList<QGraphicsItem *> items = scene()->items();
            QString tmp = 0;
            for(auto i: items){
                if (dynamic_cast<QGraphicsEllipseItem*>(i)) {
                    QGraphicsEllipseItem *ellipse = qgraphicsitem_cast<ObservableCircle*>(i);
                    if(i->scenePos().y() > scenePos().y()/* && ellipse->data(CurrentDirection) == Right*/){
                        tmp+="1";
                    }else{
                        tmp+="0";
                    }
                }
            }
            BCGen(tmp, name);
            QFile file("Info/Statistics.txt");
            QTextStream out(&file);
            if(file.open(QIODevice::Text | QIODevice::ReadWrite)){
                {
                    QString tname = name;
                    name.clear();
                    for(int i = 0;i < tname.size();i++){
                        if(tname[i] != '\n' && tname[i] != '\r'){
                            name+=tname[i];
                        }
                    }
                    qDebug()<<name<<" - change completed";
                }
                name+="\r\n";
                while(!file.atEnd()){
                    QString s = file.readLine();
                    QStringList lst = s.split(" ");
                    if(s == name){
                        s = file.readLine();
                        s = file.readLine();
                        lst = s.split(" ");
                        int kol;
                        qDebug()<<s;
                        kol = lst.at(2).toInt() + 1;
                        qDebug()<<kol;
                        QString kols;
                        kols = QString::number(kol);
                        file.seek(file.pos() - s.length()-1);
                        s.clear();
                        s.insert(0, lst.at(0));
                        s.insert(s.size()," ");
                        s.insert(s.size(), lst.at(1));
                        s.insert(s.size()," ");
                        s.insert(s.size(), kols);
                        s.insert(s.size(), " \r\n");
                        out << s;
                        break;
                    }
                }
            }else{
                qDebug()<<"Can't open file";
            }
            file.close();
        }
        delete msgBox;
	}
}

void ObservableRect::BCGen(QString number, QString name)
{
        BarcodePrinter *printer = new BarcodePrinter();
        //List available printers
        qDebug() << "Available printers: " << printer->getAvailablePrinters();
        //Configure printer (no argument means it prints pdf)
        printer->configurePrinter("");//HP LaserJet 1020
        //printer->configurePrinter("HP LaserJet 1020");
        //Print barcode
        printer->printBarcode(number, name);
}

ObservableCircle::ObservableCircle(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsEllipseItem(rect, parent)
{
    this->setData(CurrentDirection, Right);
}

void ObservableCircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
    event->accept();
    previousPosition = event->scenePos();
}

void ObservableCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    event->accept();
    //if (event->scenePos() == previousPosition && (flags() & QGraphicsItem::ItemIsMovable)) {
    if(isSelected()){
        switch( CurrentDirection )
        {
         case( Right ): CurrentDirection = Down; break;
         case( Down ): CurrentDirection = Left; break;
         case( Left ): CurrentDirection = Up; break;
         case( Up ): CurrentDirection = Right; break;
        }
    }
}
