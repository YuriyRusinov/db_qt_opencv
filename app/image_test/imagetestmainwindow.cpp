#include <QMdiArea>
#include <QtDebug>

#include <db_opencv_singleton.h>
#include <opencvcore.h>
#include <opencv_database.h>
#include <opencv_db_result.h>

#include "imagetestmainwindow.h"
#include "ui_image_test_main_window.h"

ImageTestMainWindow::ImageTestMainWindow(QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
    _UI( new Ui::Image_test_main_window ),
    _mMdiArea( new QMdiArea ),
    _mDbOpenCv( dbOpenCvS::getDBOpenCVSingleton() )
{
    _UI->setupUi( this );
    setCentralWidget( _mMdiArea );

    connect( _UI->act_Connect, &QAction::triggered, this, &ImageTestMainWindow::dbConnect );
    connect( _UI->actionView_images, &QAction::triggered, this, &ImageTestMainWindow::viewImages );
    connect( _UI->act_Quit, &QAction::triggered, this, &ImageTestMainWindow::close );
}

ImageTestMainWindow::~ImageTestMainWindow() {
    delete _UI;
}

void ImageTestMainWindow::dbConnect() {
    qDebug() << __PRETTY_FUNCTION__;
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    cvCore->GUIConnect();
}

void ImageTestMainWindow::close() {
    qDebug() << __PRETTY_FUNCTION__;
    _mDbOpenCv->reset();
    QMainWindow::close();
}

void ImageTestMainWindow::viewImages() {
    OpenCVCore* cvCore = _mDbOpenCv->getCore();
    OpenCVDatabase* db = cvCore->getDb();

    CVDbResult * res = db->execute("select id, name from images;");
    int n = res->getRowCount();
    for(int i=0; i<n; i++) {
        qDebug() << __PRETTY_FUNCTION__ << res->getCellAsInt(i, 0) << ' ' << QString(res->getCellAsString(i, 1).c_str());
    }
}
