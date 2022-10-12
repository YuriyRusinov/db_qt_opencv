#include <QBuffer>
#include <QtDebug>

#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include <opencv_db_result.h>
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
        const char* imBytes = res->getCellData(i, 2);
        QByteArray imageBytes = res->getCellAsByteArray(i, 2);
        int nn = imageBytes.size();

        QImage im;
        bool isLoaded = im.loadFromData( imageBytes );
        qDebug() << __PRETTY_FUNCTION__ << nn << isLoaded;
        resImages.insert(id, dbImages(id, name, im));
    }
    return resImages;
}
