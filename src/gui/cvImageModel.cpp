#include <QPixmap>
#include <AircraftImage.h>
#include "cvImageModel.h"

cvImageModel::cvImageModel( const QMap<long long, shared_ptr<AircraftImages>>& cvIms, QObject *parent )
    : QAbstractItemModel( parent ),
    _images( cvIms )
{
}

cvImageModel::~cvImageModel() {
}

QModelIndex	cvImageModel::index( int row, int column, const QModelIndex& parent ) const {
    if( parent.isValid() || column < 0 || column >= 3 || row <0 || row >= _images.size() )
        return QModelIndex();

    QMap< long long, shared_ptr< AircraftImages > >::const_iterator pim = _images.constBegin()+row;
    return createIndex( row, column, (quintptr)pim.key() );
}

QModelIndex	cvImageModel::parent( const QModelIndex& index ) const {
    Q_UNUSED( index );
    return QModelIndex();
}

int	cvImageModel::rowCount( const QModelIndex& parent ) const {
    if( parent.isValid() )
        return 0;
    else
        return _images.size();
}

int	cvImageModel::columnCount( const QModelIndex& parent ) const {
    if( parent.isValid() )
        return 0;
    else
        return 3;
}

QVariant cvImageModel::data( const QModelIndex& index, int role ) const {
    if( !index.isValid() )
        return QVariant();

    int iRow = index.row();
    int iCol = index.column();
    QMap< long long, shared_ptr< AircraftImages > >::const_iterator pim = _images.constBegin()+iRow;
    if( role == Qt::UserRole || (iCol == 0 && role == Qt::DisplayRole) ) {
        return pim.key();
    }
    else if( role == Qt::DisplayRole && iCol == 1 ) {
        return pim.value()->getName();
    }
    else if( role == Qt::DecorationRole && iCol == 2 ) {
        QImage im = pim.value()->getImage();
        QSize imSize = im.size();
        QSize imPreviewSize( /*imSize.width()/imSize.height()**/128, 64 );
        QImage imSc = im.scaled( imPreviewSize );
        return QPixmap::fromImage( imSc );
    }
    return QVariant();
}

QVariant cvImageModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    QStringList headers;
    headers << tr("ID")
            << tr("Image name")
            << tr("Image");
    if( section >= 0 && section < 3 && orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return headers[section];

    return QVariant();
}
