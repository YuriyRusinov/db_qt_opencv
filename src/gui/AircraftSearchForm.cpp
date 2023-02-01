#include <QFileDialog>

#include "AircraftSearchForm.h"
#include "ui_aircraft_search_form.h"

AircraftSearchForm::AircraftSearchForm( QWidget* parent, Qt::WindowFlags flags )
    : QDialog( parent, flags ),
    _UI( new Ui::aircraft_search_form ),
    m_SearchImage( QImage() ) {
    _UI->setupUi( this );

    connect( _UI->pbLoad, &QAbstractButton::clicked, this, &AircraftSearchForm::loadAircraftImage );
}

AircraftSearchForm::~AircraftSearchForm() {
    delete _UI;
}

void AircraftSearchForm::loadAircraftImage() {
    QString imageFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Images (*bmp *.jpg *jpeg *.png *ppm *xbm *.xpm);;All files (*)") );
    if( imageFileName.isEmpty() )
        return;

    m_SearchImage = QImage( imageFileName );
    _UI->lImage->setScaledContents(true);
    QPixmap pix = QPixmap::fromImage(m_SearchImage);
    _UI->lImage->setPixmap(pix);
}
