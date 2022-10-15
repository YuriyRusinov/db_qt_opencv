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

QMap<long, dbImages> dbLoader::loadImages() {
    QString SQL("select * from GetImages(null);");
    CVDbResult * res = m_db->execute( SQL.toStdString().c_str() );

    if( res == nullptr || res->getRowCount() == 0 ) {
        if( res )
            delete res;
        return QMap<long, dbImages>();
    }
    int n = res->getRowCount();
    QMap<long, dbImages> resImages;
    for(int i=0; i<n; i++) {
        long id = res->getCellAsInt(i, 0);
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
        resImages.insert(id, dbImages(id, name, im));
    }
    return resImages;
}
