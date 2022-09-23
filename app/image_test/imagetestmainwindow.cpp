#include <QMdiArea>
#include <QtDebug>

#include <db_opencv_singleton.h>

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
    connect( _UI->act_Quit, &QAction::triggered, this, &ImageTestMainWindow::close );
}

ImageTestMainWindow::~ImageTestMainWindow() {
    delete _UI;
}

void ImageTestMainWindow::dbConnect() {
    qDebug() << __PRETTY_FUNCTION__;
}

void ImageTestMainWindow::close() {
    qDebug() << __PRETTY_FUNCTION__;
    QMainWindow::close();
}
