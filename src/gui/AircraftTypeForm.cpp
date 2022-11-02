#include <QtDebug>

#include <AircraftType.h>
#include "AircraftTypeForm.h"
#include "ui_aircraft_type_form.h"

AirCraftTypeForm::AirCraftTypeForm( shared_ptr< AircraftType > aircType, QWidget* parent, Qt::WindowFlags flags )
    : QDialog( parent, flags ),
    _UI( new Ui::aircraft_type_form ),
    m_aircraftType( aircType ) {
    _UI->setupUi( this );

    if( m_aircraftType != nullptr ) {
        _UI->lETypeName->setText( m_aircraftType->getName() );
        _UI->tETypeDescription->setPlainText( m_aircraftType->getDescription() );
    }
    _UI->tbClear->setToolTip( tr("Clear parent type") );
    _UI->tbSelect->setToolTip( tr("Set parent type") );
    connect( _UI->tbClear, &QToolButton::clicked, this, &AirCraftTypeForm::clearParentType );
    connect( _UI->tbSelect, &QToolButton::clicked, this, &AirCraftTypeForm::setParentType );
}

AirCraftTypeForm::~AirCraftTypeForm() {
    delete _UI;
}

shared_ptr< AircraftType > AirCraftTypeForm::getType() const {
    m_aircraftType->setName( _UI->lETypeName->text() );
    m_aircraftType->setDescription( _UI->tETypeDescription->toPlainText() );
    return m_aircraftType;
}

void AirCraftTypeForm::setParentType() {
    qDebug() << __PRETTY_FUNCTION__;
}

void AirCraftTypeForm::clearParentType() {
    qDebug() << __PRETTY_FUNCTION__;
}
