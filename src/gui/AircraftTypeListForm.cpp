#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QMessageBox>
#include <QtDebug>

#include <AircraftType.h>
#include "AircraftTypeListForm.h"
#include "ui_aircraft_type_list_form.h"

AircraftTypeListForm::AircraftTypeListForm( QWidget* parent, Qt::WindowFlags flags )
    : QDialog( parent, flags ),
    _UI( new Ui::aircraft_type_list_form )
{
    _UI->setupUi( this );

    _UI->tbAdd->setToolTip( tr("Add new type") );
    _UI->tbAdd->setIcon( QIcon(":/icons/add.png") );
    connect( _UI->tbAdd, &QAbstractButton::clicked, this, &AircraftTypeListForm::addType );
    _UI->tbEdit->setToolTip( tr("Edit selected type") );
    _UI->tbEdit->setIcon( QIcon(":/icons/edit.png") );
    connect( _UI->tbEdit, &QAbstractButton::clicked, this, &AircraftTypeListForm::editType );
    _UI->tbDel->setToolTip( tr("Delete selected type") );
    _UI->tbDel->setIcon( QIcon(":/icons/del.png") );
    connect( _UI->tbDel, &QAbstractButton::clicked, this, &AircraftTypeListForm::delType );
    _UI->tbRefresh->setToolTip( tr("Refresh types from DB") );
    _UI->tbRefresh->setIcon( QIcon(":/icons/refresh.png") );
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
    QItemSelection selTypes = _UI->tvAircraftTypes->selectionModel()->selection();
    long long idParentType(-1);
    if( !selTypes.empty() ) {
        idParentType = selTypes.indexes().at(0).data(Qt::UserRole).toLongLong();
    }
    qDebug() << __PRETTY_FUNCTION__ << idParentType;
    emit addAircraftType( idParentType );
}

void AircraftTypeListForm::editType() {
    qDebug() << __PRETTY_FUNCTION__;
    QItemSelection selTypes = _UI->tvAircraftTypes->selectionModel()->selection();
    if( selTypes.empty() ) {
        QMessageBox::warning( this, tr("Edit type"), tr("Select type for edit"), QMessageBox::Ok );
        return;
    }
    long long idType = selTypes.indexes().at(0).data(Qt::UserRole).toLongLong();
    emit editAircraftType( idType );
}

void AircraftTypeListForm::delType() {
    qDebug() << __PRETTY_FUNCTION__;
    QItemSelection selTypes = _UI->tvAircraftTypes->selectionModel()->selection();
    if( selTypes.empty() ) {
        QMessageBox::warning( this, tr("Delete type"), tr("Select type for delete"), QMessageBox::Ok );
        return;
    }
    
    QModelIndex tIndex = selTypes.indexes().at(0);
    QAbstractItemModel* tModel = _UI->tvAircraftTypes->model();
    if( tModel->rowCount( tIndex ) > 0) {
        QMessageBox::warning( this, tr("Delete type"), tr("Cannot delete type with subtypes"), QMessageBox::Ok );
        return;
    }
    int res = QMessageBox::question( this, tr("Delete type"), tr("Do you really want to delete?"), QMessageBox::Yes | QMessageBox::No );
    long long id = tIndex.data( Qt::UserRole ).toLongLong();
    if( res == QMessageBox::Yes )
        emit delAircraftType( id );
}

void AircraftTypeListForm::refreshTypes() {
    qDebug() << __PRETTY_FUNCTION__;
    emit refreshAircraftModel( _UI->tvAircraftTypes );
}

AircraftType* AircraftTypeListForm::getType() const {
    QItemSelection selTypes = _UI->tvAircraftTypes->selectionModel()->selection();
    if( selTypes.empty() ) {
        return nullptr;
    }

    QModelIndex tIndex = selTypes.indexes().at(0);
    AircraftType* resType = tIndex.data( Qt::UserRole+1 ).value<AircraftType*>();
    return resType;
}
