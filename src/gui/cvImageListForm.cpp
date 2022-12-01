#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QModelIndex>
#include <QMessageBox>
#include <QtDebug>

#include "cvImageListForm.h"
#include "ui_cv_image_list_form.h"

cvImageListForm::cvImageListForm( QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::cv_image_list_form )
{
    _UI->setupUi( this );

    _UI->tbAddImage->setIcon( QIcon(":/icons/add.png") );
    _UI->tbAddImage->setToolTip( tr("Add new image to database" ) );
    _UI->tbUpdateImage->setIcon( QIcon(":/icons/edit.png") );
    _UI->tbUpdateImage->setToolTip( tr("Update/replace selected image in database") );
    _UI->tbDelImage->setIcon( QIcon(":/icons/del.png") );
    _UI->tbDelImage->setToolTip( tr("Delete selected image in database") );
    _UI->tbViewOpenCV->setIcon( QIcon(":/icons/view.png") );
    _UI->tbViewOpenCV->setToolTip( tr("View image in opencv widget") );
    _UI->tbRefresh->setIcon( QIcon(":/icons/refresh.png") );
    _UI->tbRefresh->setToolTip( tr("Refresh images") );

    connect( _UI->tbAddImage, &QToolButton::clicked, this, &cvImageListForm::addImage );
    connect( _UI->tbUpdateImage, &QToolButton::clicked, this, &cvImageListForm::updImage );
    connect( _UI->tbDelImage, &QToolButton::clicked, this, &cvImageListForm::delImage );
    connect( _UI->tbRefresh, &QToolButton::clicked, this, &cvImageListForm::refreshImages );
    connect( _UI->tbViewOpenCV, &QToolButton::clicked, this, &cvImageListForm::viewImage );
}

cvImageListForm::~cvImageListForm() {
    delete _UI;
}

void cvImageListForm::setImagesModel( QAbstractItemModel* imModel ) {
    QAbstractItemModel* oldModel = _UI->tvImages->model();
    _UI->tvImages->setModel( imModel );
    if( oldModel != nullptr && oldModel != imModel )
        delete oldModel;
}

void cvImageListForm::setImagesItemDelegate( QAbstractItemDelegate* imDelegate ) {
    QAbstractItemDelegate* oldDeleg = _UI->tvImages->itemDelegate();
    _UI->tvImages->setItemDelegate( imDelegate );
    if( oldDeleg && oldDeleg != imDelegate)
        delete oldDeleg;
}

void cvImageListForm::addImage( ) {
    qDebug() << __PRETTY_FUNCTION__;
    emit insertImage();
}

void cvImageListForm::updImage( ) {
    qDebug() << __PRETTY_FUNCTION__;
    QItemSelectionModel *selModel = _UI->tvImages->selectionModel();
    QItemSelection sel = selModel->selection();
    QModelIndexList selIndexes = sel.indexes();
    if( selIndexes.isEmpty() ) {
        QMessageBox::warning( this, tr("Update image"), tr("Select image for update"), QMessageBox::Ok );
        return;
    }
    QModelIndex wIndex = selIndexes[0];
    qlonglong idImage = wIndex.data( Qt::UserRole ).toLongLong();
    emit updateImage( idImage );
}

void cvImageListForm::delImage( ) {
    qDebug() << __PRETTY_FUNCTION__;
    QItemSelectionModel *selModel = _UI->tvImages->selectionModel();
    QItemSelection sel = selModel->selection();
    QModelIndexList selIndexes = sel.indexes();
    if( selIndexes.isEmpty() ) {
        QMessageBox::warning( this, tr("Delete image"), tr("Select image for remove"), QMessageBox::Ok );
        return;
    }
    QMessageBox::StandardButton isOk = QMessageBox::question( this, tr("Delete image"), tr("Do you really want to delete image ?"), QMessageBox::Yes | QMessageBox::No );
    if( isOk == QMessageBox::Yes ) {
        QModelIndex wIndex = selIndexes[0];
        qlonglong idImage = wIndex.data( Qt::UserRole ).toLongLong();
        emit deleteImage( idImage );
    }
}

void cvImageListForm::refreshImages( ) {
    qDebug() << __PRETTY_FUNCTION__;
    emit refreshModel( );
}

void cvImageListForm::viewImage( ) {
    qDebug() << __PRETTY_FUNCTION__;
    QItemSelectionModel *selModel = _UI->tvImages->selectionModel();
    QItemSelection sel = selModel->selection();
    QModelIndexList selIndexes = sel.indexes();
    if( selIndexes.isEmpty() ) {
        QMessageBox::warning( this, tr("View image"), tr("Select image for view"), QMessageBox::Ok );
        return;
    }
    QModelIndex wIndex = selIndexes[0];
    qlonglong idImage = wIndex.data( Qt::UserRole ).toLongLong();
    emit viewDbImage( idImage );
}
