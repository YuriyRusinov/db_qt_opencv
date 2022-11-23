#include <QFileDialog>
#include <QToolButton>
#include <QtDebug>

#include <AircraftType.h>
#include <AircraftImage.h>
#include "cvImageForm.h"
#include "ui_cv_image_form.h"

using std::make_shared;

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
    connect( _UI->tbSelect, &QToolButton::clicked, this, &cvImageForm::classifyImage );
    connect( _UI->tbClear, &QToolButton::clicked, this, &cvImageForm::clearType );
}

cvImageForm::~cvImageForm() {
    delete _UI;
}

void cvImageForm::loadImageFromFile() {
    QString imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg);;All files (*)") );
    if( imageFileName.isEmpty() )
        return;

    m_Image = QImage(imageFileName);
    m_aircraftImage->setImage( m_Image );
    QPixmap pix = QPixmap::fromImage(m_Image);
    _UI->lImage->setPixmap(pix);
}

void cvImageForm::saveImageToDb() {
    QString imName = _UI->lEImageName->text();
    m_aircraftImage->setName( imName );
    emit saveImage( m_aircraftImage );//, imName, m_id );
}

void cvImageForm::classifyImage() {
    qDebug() << __PRETTY_FUNCTION__;
    QString imName = _UI->lEImageName->text();
    emit setAircraftType ( m_aircraftImage );
}

void cvImageForm::setImageType( shared_ptr< AircraftType > aType ) {
    if( aType == nullptr ) {
        clearType();
        return;
    }
    _UI->lEType->setText( aType->getName() );
}

void cvImageForm::clearType() {
    _UI->lEType->clear();
    m_aircraftImage->setType( nullptr );
}
