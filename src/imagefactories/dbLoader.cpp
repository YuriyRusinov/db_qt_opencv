#include <QBuffer>
#include <QFile>
#include <QDataStream>
#include <QtDebug>

#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include <opencv_db_result.h>

#include <pqxx/binarystring.hxx>
#include "dbLoader.h"

dbLoader::dbLoader( OpenCVDatabase* db, QObject* parent )
    : QObject( parent ), 
    m_db( db ) {
}

dbLoader::~dbLoader() {

}

QMap<long long, shared_ptr< dbImages > > dbLoader::loadImages() const {
    QString SQL("select * from GetImages(null);");
    CVDbResult * res = m_db->execute( SQL.toStdString().c_str() );

    if( res == nullptr || res->getRowCount() == 0 ) {
        if( res )
            delete res;
        return QMap<long long, shared_ptr< dbImages >>();
    }
    int n = res->getRowCount();
    QMap<long long, shared_ptr< dbImages >> resImages;
    for(int i=0; i<n; i++) {
        long long id = res->getCellAsInt64(i, 0);
        QString name = QString::fromStdString( res->getCellAsString(i, 1) );
        pqxx::binarystring imBytesStr = res->getCellAsBinaryString(i, 2);
        const char* imBytes = imBytesStr.get();//res->getCellData(i, 2);
        int nn = res->getCellLength(i, 2);//imageBytes.size();
        QByteArray imageBytes = QByteArray::fromRawData( imBytes, nn );
        QByteArray ba = res->getCellAsByteArray(i, 2);
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
        shared_ptr< dbImages > pImage ( new dbImages(id, name, im) );
        qDebug() << __PRETTY_FUNCTION__ << nn << isLoaded;// << (ba.compare ( imageBytes) );
        resImages.insert(id, pImage);
    }
    delete res;
    return resImages;
}

shared_ptr< dbImages > dbLoader::loadImage( qlonglong id ) const {
    QString SQL = QString("select * from GetImages( %1 );").arg( id );
    CVDbResult * res = m_db->execute( SQL.toStdString().c_str() );

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
    pqxx::binarystring imBytesStr = res->getCellAsBinaryString(i, 2);
    const char* imBytes = imBytesStr.get();//res->getCellData(i, 2);
    int nn = res->getCellLength(i, 2);//imageBytes.size();
    QByteArray imageBytes = QByteArray::fromRawData( imBytes, nn );
    QByteArray ba = res->getCellAsByteArray(i, 2);
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
    shared_ptr< dbImages > resImage ( new dbImages(id, name, im) );
    delete res;
    return resImage;
}
