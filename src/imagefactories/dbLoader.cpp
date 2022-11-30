#include <QBuffer>
#include <QFile>
#include <QDataStream>
#include <QtDebug>

#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include <opencv_db_result.h>

#include <pqxx/binarystring.hxx>
#include <pqxx/except.hxx>
#include "dbLoader.h"

dbLoader::dbLoader( OpenCVDatabase* db, QObject* parent )
    : QObject( parent ), 
    m_db( db ) {
}

dbLoader::~dbLoader() {

}

QMap<long long, shared_ptr< AircraftImage > > dbLoader::loadImages() const {
    QString SQL("select * from GetImages(null);");
    qDebug() << __PRETTY_FUNCTION__ << SQL;
    CVDbResult * res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch(pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << "Error in images " << e.what();
        if( res )
            delete res;
        return QMap<long long, shared_ptr< AircraftImage >>();
    }

    if( res == nullptr || res->getRowCount() == 0 ) {
        if( res )
            delete res;
        return QMap<long long, shared_ptr< AircraftImage >>();
    }
    int n = res->getRowCount();
    QMap<long long, shared_ptr< AircraftImage >> resImages;
    for(int i=0; i<n; i++) {
        long long id = res->getCellAsInt64(i, 0);
        long long idType = res->getCellAsInt64(i, 1);
        QString tName = QString::fromStdString( res->getCellAsString(i, 2) );
        shared_ptr< AircraftType > aType = make_shared< AircraftType>( idType, tName );
        QString name = QString::fromStdString( res->getCellAsString(i, 3) );
        QImage aircraftImage = res->getCellAsImage( i, 4 );
        shared_ptr< AircraftImage > pImage ( new AircraftImage(id, name, aircraftImage ) );
        pImage->setType( aType );
        resImages.insert(id, pImage);
    }
    delete res;
    return resImages;
}

shared_ptr< AircraftImage > dbLoader::loadImage( qlonglong id ) const {
    QString SQL = QString("select * from GetImages( %1 );").arg( id );
    CVDbResult * res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch(pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return nullptr;
    }

    //qDebug() << __PRETTY_FUNCTION__ << (res != nullptr ? res->getRowCount() : -1) << SQL;
    if( res == nullptr || res->getRowCount() != 1 ) {
        if( res )
            delete res;
        return nullptr;
    }
    int n = res->getRowCount();
    //qDebug() << __PRETTY_FUNCTION__ << n;
    int i = 0;
    long id0 = res->getCellAsInt(i, 0);
    if( id0 != id ) {
        delete res;
        return nullptr;
    }
    long long idType = res->getCellAsInt64(i, 1);
    QString tName = QString::fromStdString( res->getCellAsString(i, 2) );
    shared_ptr< AircraftType > aType = make_shared< AircraftType>( idType, tName );
    QString name = QString::fromStdString( res->getCellAsString(i, 3) );
    QImage im = res->getCellAsImage(i, 4);
    shared_ptr< AircraftImage > resImage ( new AircraftImage(id, name, im) );
    resImage->setType( aType );
    delete res;
    return resImage;
}

map< long long, shared_ptr< AircraftType > > dbLoader::loadTypes() const {
    QString SQL("select * from GetTypes(null);");
    map< long long, shared_ptr< AircraftType >> resTypes;
    CVDbResult * res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch(pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return resTypes;
    }

    if( res == nullptr || res->getRowCount() == 0 ) {
        if( res )
            delete res;
        return resTypes;
    }
    int n = res->getRowCount();
    shared_ptr< AircraftType > currentType( nullptr );
    for(int i=0; i<n; i++) {
        long long id = res->getCellAsInt64(i, 0);
        long long idParent = res->getCellAsInt64(i, 1);
        QString tName = QString::fromStdString( res->getCellAsString(i, 2) );
        QString tDesc = QString::fromStdString( res->getCellAsString(i, 3) );
        qDebug() << __PRETTY_FUNCTION__ << id << idParent << tName << tDesc;
        if( idParent == 0 ) {
            shared_ptr< AircraftType > aType ( new AircraftType( id, tName, tDesc ) );
            currentType = aType;
            resTypes.insert( std::pair<long long, shared_ptr< AircraftType >>( id, aType ));
        }
        else {
            shared_ptr< AircraftType > pType( currentType );
            for( ;pType->getId() != idParent; pType = pType->getParent() )
                ;
            shared_ptr< AircraftType > aType ( new AircraftType( id, tName, tDesc ) );
            if( pType != nullptr ) {
                aType->setParent( pType );
                pType->childTypes().insert( std::pair<long long,shared_ptr< AircraftType >>( id, aType ));
            }
            currentType = aType;
        }
    }

    for(map< long long,shared_ptr< AircraftType > >::const_iterator pres = resTypes.cbegin();
        pres != resTypes.cend();
        pres++ )
        qDebug() << __PRETTY_FUNCTION__ << pres->first << ' ' << pres->second->childTypes().size();

    delete res;
    return resTypes;
}

shared_ptr< AircraftType > dbLoader::loadType( long long idType ) const {
    QString SQL = QString("select * from GetType( %1 );").arg(idType);
    shared_ptr< AircraftType > resType = nullptr;
    CVDbResult * res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch(pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return resType;
    }

    if( res == nullptr || res->getRowCount() != 1 ) {
        if( res )
            delete res;
        return resType;
    }
    int n = res->getRowCount();
    int i=0;
    long long id = res->getCellAsInt64(i, 0);
    long long idParent = res->getCellAsInt64(i, 1);
    QString tName = QString::fromStdString( res->getCellAsString(i, 2) );
    QString tDesc = QString::fromStdString( res->getCellAsString(i, 3) );
    qDebug() << __PRETTY_FUNCTION__ << id << idParent << tName << tDesc;
    if( idParent == 0 ) {
        resType = make_shared< AircraftType >( id, tName, tDesc );
    }
    else {
        shared_ptr< AircraftType > parentType = loadType( idParent );//( currentType );
        resType = make_shared< AircraftType > ( id, tName, tDesc );
        resType->setParent( parentType );
    }

    delete res;
    return resType;
}
