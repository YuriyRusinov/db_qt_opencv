#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPixmap>
#include <QTreeView>
#include <QStandardItemModel>
#include <QtDebug>

#include <iostream>
#include <fstream>

#include <pqxx/binarystring.hxx>
#include <pqxx/except.hxx>

#include <db_opencv_singleton.h>
#include <opencvcore.h>
#include <opencv_database.h>
#include <opencv_db_result.h>
#include <dbLoader.h>
#include <dbWriter.h>
#include <dbImageClass.h>

#include "imagetestmainwindow.h"
#include "ui_image_test_main_window.h"

using std::ios_base;

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
    connect( _UI->act_Disconnect, &QAction::triggered, this, &ImageTestMainWindow::dbDisconnect );
    connect( _UI->actionfrom_Directory, &QAction::triggered, this, &ImageTestMainWindow::importFromDir );
    connect( _UI->actionfrom_Camera, &QAction::triggered, this, &ImageTestMainWindow::importFromCamera );
    connect( _UI->actionfrom_URL, &QAction::triggered, this, &ImageTestMainWindow::importFromURL );
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

void ImageTestMainWindow::dbDisconnect() {
    qDebug() << __PRETTY_FUNCTION__;
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    cvCore->dbDisconnect();
    setActionsEnable( false );
}

void ImageTestMainWindow::close() {
    qDebug() << __PRETTY_FUNCTION__;
    _mDbOpenCv->reset();
    QMainWindow::close();
}

void ImageTestMainWindow::viewImages() {
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    cvCore->GUIViewImages();
}

void ImageTestMainWindow::init() {
}

void ImageTestMainWindow::setActionsEnable( bool enable ) {
    _UI->actInsert_image->setEnabled( enable );
    _UI->actionView_images->setEnabled( enable );
    _UI->menuImport->setEnabled( enable );
    _UI->act_Disconnect->setEnabled( enable );
}

void ImageTestMainWindow::insertImage() {
    qDebug() << __PRETTY_FUNCTION__;
    QString imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images (*bmp *.jpg *jpeg *.png *ppm *xbm *.xpm);;All files (*)") );
    if( imageFileName.isEmpty() )
        return;

    QImage im(imageFileName);
/*
 *   For debug
 *
    im.save("ttt.jpg", "JPG");
    QByteArray ba;
    QBuffer bbb( &ba );
    bbb.open( QBuffer::WriteOnly );

    im.save( &bbb, "JPG" );
    bbb.close();

    QImage im1;
    bool isLoaded = im1.loadFromData( ba );
    qDebug() << __PRETTY_FUNCTION__ << isLoaded << im1.isNull();
    im1.save("ttt_1.jpg", "JPG");

    const void* pBa ( (const void *)ba );
    pqxx::binarystring pqStr( pBa, ba.size() );
    qDebug() << __PRETTY_FUNCTION__ << ba.size() << ba.toStdString().size() << pqStr.size();
    QByteArray ba1 = QByteArray::fromRawData( pqStr.get(),  pqStr.size() );
    QFile fByteArr("ttt_by.bin");
    fByteArr.open( QBuffer::WriteOnly );
    QDataStream fByteArrStr( &fByteArr );
    fByteArrStr << ba1;
    QImage im2;
    bool isLoaded2 = im2.loadFromData( ba1 );
    qDebug() << __PRETTY_FUNCTION__ << isLoaded2 << ba1.size();
    QFile tstIm("ttt_2.jpg");
    tstIm.open( QBuffer::WriteOnly );
    im2.save( &tstIm );
    tstIm.close();
*/
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    cvCore->loadImage( -1, QString(), im );
}

void ImageTestMainWindow::addSubWindow( QWidget* w ) {
    if(w == nullptr)
        return;
    QMdiSubWindow * mdiW = _mMdiArea->addSubWindow( w );
    mdiW->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void ImageTestMainWindow::importFromDir() {
    qDebug() << __PRETTY_FUNCTION__;
    QStringList fileNames = QFileDialog::getOpenFileNames( this, tr("Open images"), QDir::currentPath(), tr("Images (*bmp *.jpg *jpeg *.png *ppm *xbm *.xpm);;All files (*)") );
    if( fileNames.isEmpty() )
        return;
    int nFiles = fileNames.size();
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    shared_ptr< dbWriter > dbw = cvCore->getDbWriter();
    for(int i=0; i<nFiles; i++) {
        qDebug() << __PRETTY_FUNCTION__ << fileNames[i];
        QImage im (fileNames[i]);
        QString imName = fileNames[i];
        try {
            dbw->insertImage(im, imName);
        }
        catch(pqxx::failure& e) {
            qDebug() << __PRETTY_FUNCTION__ << e.what();
            continue;
        }
    }
}

void ImageTestMainWindow::importFromCamera() {
    qDebug() << __PRETTY_FUNCTION__;
}

void ImageTestMainWindow::importFromURL() {
    qDebug() << __PRETTY_FUNCTION__;
}
