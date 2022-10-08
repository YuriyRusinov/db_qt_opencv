#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPixmap>
#include <QtDebug>

#include <db_opencv_singleton.h>
#include <opencvcore.h>
#include <opencv_database.h>
#include <opencv_db_result.h>
#include <dbLoader.h>
#include <dbWriter.h>

#include "imagetestmainwindow.h"
#include "ui_image_test_main_window.h"

ImageTestMainWindow::ImageTestMainWindow(QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
    _UI( new Ui::Image_test_main_window ),
    _mMdiArea( new QMdiArea ),
    _mDbOpenCv( dbOpenCvS::getDBOpenCVSingleton() ),
    _isDbConnected( false )
{
    _UI->setupUi( this );
    setCentralWidget( _mMdiArea );
    setActionsEnable( false );
    OpenCVCore* cvCore = _mDbOpenCv->getCore();

    connect( _UI->act_Connect, &QAction::triggered, this, &ImageTestMainWindow::dbConnect );
    connect( _UI->actionView_images, &QAction::triggered, this, &ImageTestMainWindow::viewImages );
    connect( cvCore, &OpenCVCore::setWidget, this, &ImageTestMainWindow::addSubWindow );
    connect( _UI->actInsert_image, &QAction::triggered, this,  &ImageTestMainWindow::insertImage );
    connect( _UI->act_Quit, &QAction::triggered, this, &ImageTestMainWindow::close );
}

ImageTestMainWindow::~ImageTestMainWindow() {
    delete _UI;
}

void ImageTestMainWindow::dbConnect() {
    qDebug() << __PRETTY_FUNCTION__;
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    _isDbConnected = cvCore->GUIConnect();
    setActionsEnable( _isDbConnected );
}

void ImageTestMainWindow::close() {
    qDebug() << __PRETTY_FUNCTION__;
    _mDbOpenCv->reset();
    QMainWindow::close();
}

void ImageTestMainWindow::viewImages() {
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    OpenCVDatabase* db = cvCore->getDb();

    CVDbResult * res = db->execute("select id, name, image_bytes from images;");
    int n = res->getRowCount();
    for(int i=0; i<n; i++) {
        string chs = res->getCell(i, 2);
        const char* imByArray = res->getCellData(i, 2);
        QByteArray imBytes(chs.c_str());
        QImage im;
        im.loadFromData(imBytes);
        //QString imFile = QString("TestImage_%1.png").arg(i);
        //bool isSaved = im.save(imFile, "PNG");
        qDebug() << __PRETTY_FUNCTION__ << res->getCellAsInt(i, 0) << ' ' << QString(res->getCellAsString(i, 1).c_str()) ;
        /*QFile chsF (QString("TestImage_%1").arg(i));
        chsF.open(QIODevice::WriteOnly);
        QDataStream chsFD(&chsF);
        chsFD << imBytes;*/
    }
}

void ImageTestMainWindow::init() {
}

void ImageTestMainWindow::setActionsEnable( bool enable ) {
    _UI->actInsert_image->setEnabled( enable );
    _UI->actionView_images->setEnabled( enable );
}

void ImageTestMainWindow::insertImage() {
    qDebug() << __PRETTY_FUNCTION__;
    QString imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg);;All files (*)") );
    if( imageFileName.isEmpty() )
        return;

    QImage im(imageFileName);
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    cvCore->loadImage( im );
}

void ImageTestMainWindow::addSubWindow( QWidget* w ) {
    if(w == nullptr)
        return;
    QMdiSubWindow * mdiW = _mMdiArea->addSubWindow( w );
    mdiW->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}
