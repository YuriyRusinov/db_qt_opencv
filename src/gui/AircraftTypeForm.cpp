#include <QtDebug>

#include <AircraftType.h>
#include "AircraftTypeForm.h"
#include "ui_aircraft_type_form.h"

AircraftTypeForm::AircraftTypeForm( shared_ptr< AircraftType > aircType, QWidget* parent, Qt::WindowFlags flags )
    : QDialog( parent, flags ),
    _UI( new Ui::aircraft_type_form ),
    m_aircraftType( aircType ) {
    _UI->setupUi( this );

    if( m_aircraftType != nullptr ) {
        _UI->lETypeName->setText( m_aircraftType->getName() );
        _UI->tETypeDescription->setPlainText( m_aircraftType->getDescription() );
        if( m_aircraftType->getParent() )
            _UI->lEParentType->setText( m_aircraftType->getParent()->getName() );
    }
    _UI->tbClear->setToolTip( tr("Clear parent type") );
    _UI->tbSelect->setToolTip( tr("Set parent type") );

    connect( _UI->tbClear, &QToolButton::clicked, this, &AircraftTypeForm::clearParentType );
    connect( _UI->tbSelect, &QToolButton::clicked, this, &AircraftTypeForm::setParentType );
    connect( _UI->buttonBox, &QDialogButtonBox::accepted, this, &AircraftTypeForm::saveType );
    connect( _UI->buttonBox, &QDialogButtonBox::rejected, this, &AircraftTypeForm::close );
}

AircraftTypeForm::~AircraftTypeForm() {
    delete _UI;
}

shared_ptr< AircraftType > AircraftTypeForm::getType() const {
    m_aircraftType->setName( _UI->lETypeName->text() );
    m_aircraftType->setDescription( _UI->tETypeDescription->toPlainText() );
    return m_aircraftType;
}

void AircraftTypeForm::setParentType() {
    qDebug() << __PRETTY_FUNCTION__;
    emit selectParentType( m_aircraftType );
    if( m_aircraftType->getParent() )
        _UI->lEParentType->setText( m_aircraftType->getParent()->getName() );
}

void AircraftTypeForm::clearParentType() {
    qDebug() << __PRETTY_FUNCTION__;
    m_aircraftType->setParent( nullptr );
    _UI->lEParentType->clear();
}

void AircraftTypeForm::setType( shared_ptr< AircraftType > aircType ) {
    m_aircraftType = aircType;
    if( m_aircraftType != nullptr ) {
        _UI->lETypeName->setText( m_aircraftType->getName() );
        _UI->tETypeDescription->setPlainText( m_aircraftType->getDescription() );
        if( m_aircraftType->getParent() )
            _UI->lEParentType->setText( m_aircraftType->getParent()->getName() );
    }
}

void AircraftTypeForm::saveType() {
    qDebug() << __PRETTY_FUNCTION__;
    m_aircraftType->setName( _UI->lETypeName->text() );
    m_aircraftType->setDescription( _UI->tETypeDescription->toPlainText() );
    emit saveTypeToDb( m_aircraftType );
    setVisible(true);
}

void AircraftTypeForm::close() {
    QWidget* parentW = parentWidget();
    QDialog::close();
    if( parentW != nullptr )
        parentW->close();
}
