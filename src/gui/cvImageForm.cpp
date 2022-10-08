#include <QFileDialog>
#include <QToolButton>

#include "cvImageForm.h"
#include "ui_cv_image_form.h"

cvImageForm::cvImageForm( const QImage& image, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    m_Image( image ),
    _UI( new Ui::cv_image_form ) {
    _UI->setupUi( this );

    QPixmap pix = QPixmap::fromImage(m_Image);
    _UI->lImage->setPixmap(pix);
    _UI->tbLoadImage->setToolTip( tr("Load Image from file") );
    _UI->tbSaveImage->setToolTip( tr("Save Image to database") );

    connect( _UI->tbLoadImage, &QToolButton::clicked, this, &cvImageForm::loadImageFromFile );
    connect( _UI->tbSaveImage, &QToolButton::clicked, this, &cvImageForm::saveImageToDb );
}

cvImageForm::~cvImageForm() {
    delete _UI;
}

void cvImageForm::loadImageFromFile() {
    QString imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg);;All files (*)") );
    if( imageFileName.isEmpty() )
        return;

    m_Image = QImage(imageFileName);
    QPixmap pix = QPixmap::fromImage(m_Image);
    _UI->lImage->setPixmap(pix);
}

void cvImageForm::saveImageToDb() {
    QString imName = _UI->lEImageName->text();
    emit saveImage( m_Image, imName );
}
