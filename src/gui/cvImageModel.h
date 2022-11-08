#pragma once

#include <memory>
#include <QAbstractItemModel>
#include <QMap>

using std::shared_ptr;

class AircraftImage;

class cvImageModel : public QAbstractItemModel {
public:
    cvImageModel( const QMap<long long, shared_ptr< AircraftImage >>& cvIms = QMap<long long, shared_ptr<AircraftImage>>(), QObject *parent = nullptr );
    virtual ~cvImageModel();

    virtual QModelIndex	index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    virtual QModelIndex	parent( const QModelIndex& index ) const override;
    virtual int	rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    virtual int	columnCount( const QModelIndex& parent = QModelIndex() ) const override;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

private:
    QMap<long long, shared_ptr< AircraftImage >> _images;

private:
    Q_OBJECT
};
