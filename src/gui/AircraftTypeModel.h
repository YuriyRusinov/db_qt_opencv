#pragma once

#include <QAbstractItemModel>

#include <map>
#include <memory>

using std::map;
using std::shared_ptr;

class AircraftType;

class AircraftTypeModel : public QAbstractItemModel {
public:
    AircraftTypeModel( const map< long long, shared_ptr< AircraftType > >& airTypes, QObject* parent=nullptr );
    virtual ~AircraftTypeModel();

    virtual QModelIndex	index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    virtual QModelIndex	parent( const QModelIndex& index ) const override;
    virtual int	rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    virtual int	columnCount( const QModelIndex& parent = QModelIndex() ) const override;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

private:
    map<long long, shared_ptr< AircraftType >> m_aircraftTypes;

private:
    Q_OBJECT
};
