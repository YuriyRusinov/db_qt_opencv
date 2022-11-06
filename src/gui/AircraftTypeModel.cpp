#include <AircraftType.h>
#include "AircraftTypeModel.h"

AircraftTypeModel::AircraftTypeModel( const map< long long, shared_ptr< AircraftType > >& airTypes, QObject* parent )
    : QAbstractItemModel( parent ),
    m_aircraftTypes( airTypes )
{
}

AircraftTypeModel::~AircraftTypeModel() {}

QModelIndex	AircraftTypeModel::index( int row, int column, const QModelIndex& parent ) const {
    void *iPtr = nullptr;
    map<long long, shared_ptr<AircraftType>>::const_iterator cType;
    if( !parent.isValid() ) {
        cType = m_aircraftTypes.cbegin();
    }
    else {
        void* iParent = parent.internalPointer();
        AircraftType * aType = ( AircraftType* )iParent;
        cType = aType->childTypes().cbegin();
    }
    for(int i = 0; i<row; i++)
        cType++;
    shared_ptr< AircraftType > aType = cType->second;
    AircraftType* avType = aType.get();
    iPtr = (void *)avType;
    return createIndex( row, column, iPtr );
}

QModelIndex	AircraftTypeModel::parent( const QModelIndex& index ) const {
    void* iIndex = index.internalPointer();
    AircraftType * aType = ( AircraftType* )iIndex;
    if( aType == nullptr || aType->getParent() == nullptr )
        return QModelIndex();

    AircraftType * pType = aType->getParent().get();
    if( pType->getParent() == nullptr ) {
        long long ipId = pType->getId();
        map<long long, shared_ptr<AircraftType>>::const_iterator cType = m_aircraftTypes.find(ipId);
        int row = 0;
        for( auto it = m_aircraftTypes.cbegin(); it != cType; it++ )
            row++;
        return createIndex(row, 0, (void *)pType);
    }
    else {
        long long ipId = pType->getParent()->getId();
        map<long long, shared_ptr<AircraftType>>::const_iterator cType = pType->getParent()->childTypes().find( ipId );
        int row = 0;
        for( auto it = pType->getParent()->childTypes().cbegin(); it != cType; it++ )
            row++;
        return createIndex( row, 0, (void *)pType);
    }
}

int	AircraftTypeModel::rowCount( const QModelIndex& parent ) const {
    if( !parent.isValid() )
        return m_aircraftTypes.size();

    void* vType = parent.internalPointer();
    AircraftType * aType = ( AircraftType* )(vType);
    if( vType == nullptr || aType == nullptr )
        return 0;

    return aType->childTypes().size();
}

int	AircraftTypeModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent );
    return 3;
}

QVariant AircraftTypeModel::data( const QModelIndex& index, int role ) const {
    if( !index.isValid() )
        return QVariant();

    AircraftType* aType = (AircraftType*)index.internalPointer();
    if( aType == nullptr )
        return QVariant();

    int iCol = index.column();
    if( role == Qt::DisplayRole ) {
        switch(iCol) {
            case 0: default: return aType->getId(); break;
            case 1: return aType->getName(); break;
            case 2: { QString res = aType->getDescription();
                      QStringList resL = res.split("\n");
                      if( resL.size() == 1 )
                          return res;
                      else
                          return QString("%1...").arg( resL[0] );
                      break;
            }
        }
    }
    else if( role == Qt::UserRole )
        return aType->getId();
    else if( role == Qt::UserRole+1 )
        return QVariant::fromValue( aType );

    return QVariant();
}

QVariant AircraftTypeModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( section < 0 || section >= 3 || orientation != Qt::Horizontal || role != Qt::DisplayRole )
        return QVariant();

    QStringList headers;
    headers << tr("ID")
            << tr("Type name")
            << tr("Type description");
    return headers[section];
}
