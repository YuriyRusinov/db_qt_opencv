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

QMap<long long, shared_ptr< AircraftImages > > dbLoader::loadImages() const {
    QString SQL("select * from GetImages(null);");
    CVDbResult * res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch(pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return QMap<long long, shared_ptr< AircraftImages >>();
    }

    if( res == nullptr || res->getRowCount() == 0 ) {
        if( res )
            delete res;
        return QMap<long long, shared_ptr< AircraftImages >>();
    }
    int n = res->getRowCount();
    QMap<long long, shared_ptr< AircraftImages >> resImages;
    for(int i=0; i<n; i++) {
        long long id = res->getCellAsInt64(i, 0);
        QString name = QString::fromStdString( res->getCellAsString(i, 1) );
        pqxx::binarystring imBytesStr = res->getCellAsBinaryString(i, 4);
        const char* imBytes = imBytesStr.get();//res->getCellData(i, 2);
        int nn = res->getCellLength(i, 4);//imageBytes.size();
        QByteArray imageBytes = QByteArray::fromRawData( imBytes, nn );
        QByteArray ba = res->getCellAsByteArray(i, 4);
        QByteArray bDiff;
        QByteArray cDiff;
        for(int ii=0; ii<nn; ii++) {
            char c0 = imageBytes[ii];
            char c1 = ba[ii];
            if(c1 != c0) {
                bDiff.append(c0);
                cDiff.append(c1);
            }
        }
        qDebug() << __PRETTY_FUNCTION__ << imageBytes.compare( ba ) << (imageBytes == ba) << bDiff << cDiff;
        QImage im;
        bool isLoaded = im.loadFromData( imageBytes );
        shared_ptr< AircraftImages > pImage ( new AircraftImages(id, name, im) );
        qDebug() << __PRETTY_FUNCTION__ << nn << isLoaded;// << (ba.compare ( imageBytes) );
        resImages.insert(id, pImage);
    }
    delete res;
    return resImages;
}

shared_ptr< AircraftImages > dbLoader::loadImage( qlonglong id ) const {
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

    if( res == nullptr || res->getRowCount() != 1 ) {
        if( res )
            delete res;
        return nullptr;
    }
    int n = res->getRowCount();
    int i = 0;
    long id0 = res->getCellAsInt(i, 0);
    if( id0 != id ) {
        delete res;
        return nullptr;
    }
    QString name = QString::fromStdString( res->getCellAsString(i, 1) );
    pqxx::binarystring imBytesStr = res->getCellAsBinaryString(i, 4);
    const char* imBytes = imBytesStr.get();//res->getCellData(i, 2);
    int nn = res->getCellLength(i, 4);//imageBytes.size();
    QByteArray imageBytes = QByteArray::fromRawData( imBytes, nn );
    QByteArray ba = res->getCellAsByteArray(i, 4);
    QByteArray bDiff;
    QByteArray cDiff;
    for(int ii=0; ii<nn; ii++) {
        char c0 = imageBytes[ii];
        char c1 = ba[ii];
        if(c1 != c0) {
            bDiff.append(c0);
            cDiff.append(c1);
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << imageBytes.compare( ba ) << (imageBytes == ba) << bDiff << cDiff;
    QImage im;
    bool isLoaded = im.loadFromData( imageBytes );
    qDebug() << __PRETTY_FUNCTION__ << nn << isLoaded;// << (ba.compare ( imageBytes) );
    shared_ptr< AircraftImages > resImage ( new AircraftImages(id, name, im) );
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
