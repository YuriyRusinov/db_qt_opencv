#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QtDebug>

#include "AircraftTypeListForm.h"
#include "ui_aircraft_type_list_form.h"

AircraftTypeListForm::AircraftTypeListForm( QWidget* parent, Qt::WindowFlags flags )
    : QDialog( parent, flags ),
    _UI( new Ui::aircraft_type_list_form )
{
    _UI->setupUi( this );

    _UI->tbAdd->setToolTip( tr("Add new type") );
    connect( _UI->tbAdd, &QAbstractButton::clicked, this, &AircraftTypeListForm::addType );
    _UI->tbEdit->setToolTip( tr("Edit selected type") );
    connect( _UI->tbEdit, &QAbstractButton::clicked, this, &AircraftTypeListForm::editType );
    _UI->tbDel->setToolTip( tr("Delete selected type") );
    connect( _UI->tbDel, &QAbstractButton::clicked, this, &AircraftTypeListForm::delType );
    _UI->tbRefresh->setToolTip( tr("Refresh types from DB") );
    connect( _UI->tbRefresh, &QAbstractButton::clicked, this, &AircraftTypeListForm::refreshTypes );
}

AircraftTypeListForm::~AircraftTypeListForm() {
    delete _UI;
}

void AircraftTypeListForm::setModel( QAbstractItemModel* typesMod ) {
    QAbstractItemModel* oldModel = _UI->tvAircraftTypes->model();
    _UI->tvAircraftTypes->setModel( typesMod );
    if( oldModel != nullptr && oldModel != typesMod )
        delete oldModel;
}

void AircraftTypeListForm::viewToolButtons( bool isVisible ) {
    _UI->tbRefresh->setVisible( isVisible );
    _UI->tbAdd->setVisible( isVisible );
    _UI->tbEdit->setVisible( isVisible );
    _UI->tbDel->setVisible( isVisible );
    _UI->buttonBox->setVisible( !isVisible );
}

void AircraftTypeListForm::addType() {
    qDebug() << __PRETTY_FUNCTION__;
}

void AircraftTypeListForm::editType() {
    qDebug() << __PRETTY_FUNCTION__;
}

void AircraftTypeListForm::delType() {
    qDebug() << __PRETTY_FUNCTION__;
}

void AircraftTypeListForm::refreshTypes() {
    qDebug() << __PRETTY_FUNCTION__;
}
