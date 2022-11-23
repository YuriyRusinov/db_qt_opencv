#include <QFileDialog>
#include <QToolButton>
#include <QtDebug>

#include <AircraftType.h>
#include <AircraftImage.h>
#include "cvImageForm.h"
#include "ui_cv_image_form.h"

using std::make_shared;

cvImageForm::cvImageForm( qlonglong id, const QString& imName, const QImage& image, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    m_aircraftImage( make_shared< AircraftImage >(id, imName, image) ),
    m_id( id ),
    m_ImageName( imName ),
    m_Image( image ),
    _UI( new Ui::cv_image_form ) {
    _UI->setupUi( this );
    _UI->lEImageName->setText( m_ImageName );

    QPixmap pix = QPixmap::fromImage(m_Image);
    _UI->lImage->setPixmap(pix);
    _UI->tbLoadImage->setToolTip( tr("Load Image from file") );
    _UI->tbSaveImage->setToolTip( tr("Save Image to database") );

    connect( _UI->tbLoadImage, &QToolButton::clicked, this, &cvImageForm::loadImageFromFile );
    connect( _UI->tbSaveImage, &QToolButton::clicked, this, &cvImageForm::saveImageToDb );
}

cvImageForm::cvImageForm( shared_ptr< AircraftImage > aImage, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::cv_image_form ),
    m_aircraftImage( aImage ),
    m_id( aImage == nullptr ? -1 : aImage->getId() ),
    m_ImageName( aImage == nullptr ? QString() : aImage->getName() ),
    m_Image( aImage == nullptr ? QImage() : aImage->getImage() )
{
    _UI->setupUi( this );
    _UI->lEImageName->setText( m_ImageName );
    if( aImage && aImage->getType() )
        _UI->lEType->setText( aImage->getType()->getName() );

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
    emit saveImage( m_Image, imName, m_id );
}

void cvImageForm::classifyImage() {
    qDebug() << __PRETTY_FUNCTION__;
    QString imName = _UI->lEImageName->text();
    emit classifyImage( m_aircraftImage );//m_Image, imName, m_id );
}
