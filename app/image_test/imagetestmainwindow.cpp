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
    shared_ptr< dbLoader > dbl = cvCore->getDbLoader();
    QMap<long, dbImages> images = dbl->loadImages();
    QTreeView* tvImages = new QTreeView;
    QAbstractItemModel * imModel = new QStandardItemModel( images.size(), 3 );
    tvImages->setModel( imModel );
    imModel->setHeaderData( 0, Qt::Horizontal, tr("ID"), Qt::DisplayRole );
    imModel->setHeaderData( 1, Qt::Horizontal, tr("Image name"), Qt::DisplayRole );
    imModel->setHeaderData( 2, Qt::Horizontal, tr("Image"), Qt::DisplayRole );
    int i=0;
    for(QMap<long, dbImages>::const_iterator pim = images.constBegin();
                                             pim != images.constEnd();
                                             pim++ ) {
        imModel->setData( imModel->index(i, 0), QVariant((qlonglong)pim.key()), Qt::DisplayRole );
        imModel->setData( imModel->index(i, 1), pim.value().getName(), Qt::DisplayRole );
        QImage im = pim.value().getImage();
        imModel->setData( imModel->index(i, 2), QVariant(im), Qt::DecorationRole );
        i++;
    }
    qDebug() << __PRETTY_FUNCTION__ << imModel->rowCount();

    addSubWindow( tvImages );
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
/*    im.save("ttt.jpg", "JPG");

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
    QImage im2;
    bool isLoaded2 = im2.loadFromData( ba1 );
    qDebug() << __PRETTY_FUNCTION__ << isLoaded2 << ba1.size();
    QFile tstIm("ttt_2.jpg");
    tstIm.open( QBuffer::WriteOnly );
    im2.save( &tstIm );
    tstIm.close();
*/
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
