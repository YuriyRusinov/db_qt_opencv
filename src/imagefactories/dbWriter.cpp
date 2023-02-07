#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QDataStream>
#include <QtDebug>

#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include <opencv_db_result.h>

#include <AircraftImage.h>
#include <AircraftType.h>
#include "dbWriter.h"

using std::string;
using std::stringstream;
using std::cerr;
using std::endl;

dbWriter::dbWriter( OpenCVDatabase* db, QObject* parent )
    : QObject(parent),
    m_db( db ) {
}

dbWriter::~dbWriter( ) {
}

long long dbWriter::insertImage(const QImage& im, QString imName) {
    QByteArray baImg;
    QBuffer buffer(&baImg);
    im.save( &buffer, "JPG" );
    buffer.close();

    qDebug() << __PRETTY_FUNCTION__ << im.size() << baImg.size();
    QString SQL ("select image_insert (");
    const int nParams = 2;
    int* paramTypes = new int[nParams];
    paramTypes[0] = CVDbResult::DataType::dtVarchar;
    paramTypes[1] = CVDbResult::DataType::dtBytea;
    const char** paramValues = new const char*[nParams];
    int nameSize = imName.toUtf8().size();
    paramValues[0] = new char [nameSize];
#if PQXX_VERSION_MAJOR < 7
    strncpy( const_cast<char *>(paramValues[0]), imName.toUtf8().constData(), nameSize);
#else
    strncpy( const_cast<char *>(paramValues[0]), imName.toStdString().c_str(), nameSize);
#endif
    const void* pBa ( (const void *)baImg );
    int imgN = baImg.size();
#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring pqStr( pBa, imgN );
    paramValues[1] = pqStr.get();
#else
    paramValues[1] = new char[ imgN+1 ];
    memcpy((void *)paramValues[1], pBa, imgN );
#endif
    int* paramLength = new int[nParams];
    paramLength[0] = nameSize;
    paramLength[1] = imgN;
    int* paramFormats = new int[nParams];
    int resForm = 0;
    CVDbResult* res = nullptr;
    try {
        res = m_db->execParams( SQL.toStdString().c_str(),
                                        nParams,
                                        paramTypes,
                                        paramValues,
                                        paramLength,
                                        paramFormats,
                                        resForm );
    }
    catch( pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        delete [] paramLength;
        delete [] paramTypes;
        for(int i=0; i<nParams-1; i++) {
            //
            // I have to delete only one char*
            // because the next will be deleted
            //
            delete [] paramValues[i];
        }
        delete [] paramValues;
        throw;
    }

    delete [] paramLength;
    delete [] paramTypes;
    for(int i=0; i<nParams-1; i++) {
        //
        // I have to delete only one char*
        // because the next will be deleted
        //
        delete [] paramValues[i];
    }
    delete [] paramValues;

    long long imId = -1;
    if (res != nullptr ) {
        imId = res->getCellAsInt(0, 0);
        delete res;
    }

    return imId;
}

long long dbWriter::updateImage(const QImage& im, QString imName, qlonglong id) {
    QByteArray baImg;
    QBuffer buffer(&baImg);
    im.save( &buffer, "JPG" );
    buffer.close();

    qDebug() << __PRETTY_FUNCTION__ << im.size() << baImg.size() << imName;
    QString SQL = QString("select set_image ( ");
    const int nParams = 3;
    int* paramTypes = new int[nParams];
    paramTypes[0] = CVDbResult::DataType::dtInt8;
    paramTypes[1] = CVDbResult::DataType::dtVarchar;
    paramTypes[2] = CVDbResult::DataType::dtBytea;
    const char** paramValues = new const char*[nParams];
    stringstream idStr;
    idStr << id;

    paramValues[0] = new char [idStr.str().size()];
    strncpy( const_cast<char *>(paramValues[0]), idStr.str().c_str(), idStr.str().size() );
    qDebug() << __PRETTY_FUNCTION__ << paramValues[0] << atoll( paramValues[0] );
    int nameSize = imName.toUtf8().size();
    paramValues[1] = new char [nameSize+1];
#if PQXX_VERSION_MAJOR < 7
    strncpy( const_cast<char *>(paramValues[1]), imName.toUtf8().constData(), nameSize);
#else
    strncpy( const_cast<char *>(paramValues[1]), imName.toStdString().c_str(), nameSize);
#endif
    qDebug() << __PRETTY_FUNCTION__ << paramValues[1];
    const void* pBa ( (const void *)baImg );
    int imgN = baImg.size();
#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring pqStr( pBa, imgN );
    paramValues[2] = pqStr.get();
#else
    paramValues[2] = new char[ imgN+1 ];
    memcpy((void *)paramValues[2], pBa, imgN );
#endif
    int* paramLength = new int[nParams];
    paramLength[0] = sizeof(long long);//idStr.str().size();
    paramLength[1] = nameSize;//2*imName.size();
    paramLength[2] = imgN;
    int* paramFormats = new int[nParams];
    int resForm = 0;
    CVDbResult* res = m_db->execParams( SQL.toStdString().c_str(),
                                        nParams,
                                        paramTypes,
                                        paramValues,
                                        paramLength,
                                        paramFormats,
                                        resForm );
    delete [] paramLength;
    delete [] paramTypes;
    for(int i=0; i<nParams-1; i++) {
        //
        // I have to delete only one char*
        // because the next will be deleted
        //
        delete [] paramValues[i];
    }
    delete [] paramValues;

    long long imId = -1;
    if (res != nullptr ) {
        imId = res->getCellAsInt(0, 0);
        delete res;
    }

    return id;
}

long long dbWriter::delImage( qlonglong id ) {
    QString SQL = QString("select del_image ( %1 ); ").arg(id);
    CVDbResult* res = m_db->execute( SQL.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1) {
        if( res )
            delete res;
        return -1;
    }
    long long idd = res->getCellAsInt64(0, 0);

    delete res;
    return idd;
}

long long dbWriter::insertType( shared_ptr< AircraftType > wType ) {
    if( wType == nullptr )
        return -1;
    QString SQL = QString("select insertType( %1, '%2', '%3' );")
                        .arg( wType->getParent() ? QString::number( wType->getParent()->getId() ) : QString("NULL::bigint") )
                        .arg( wType->getName() )
                        .arg( wType->getDescription() );

    CVDbResult* res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch( pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return -1;
    }

    if( res == nullptr || res->getRowCount() != 1) {
        if( res )
            delete res;
        return -1;
    }
    long long idd = res->getCellAsInt64(0, 0);

    delete res;
    return idd;
}

long long dbWriter::updateType( shared_ptr< AircraftType > wType ) {
    if( wType == nullptr )
        return -1;
    QString SQL = QString("select updateType( %1, %2, '%3', '%4' );")
                        .arg( wType->getId() )
                        .arg( wType->getParent() ? QString::number( wType->getParent()->getId() ) : QString("NULL::bigint") )
                        .arg( wType->getName() )
                        .arg( wType->getDescription() );

    CVDbResult* res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch( pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return -1;
    }

    if( res == nullptr || res->getRowCount() != 1) {
        if( res )
            delete res;
        return -1;
    }
    long long idd = res->getCellAsInt64(0, 0);

    delete res;
    return idd;
}

long long dbWriter::delType( long long id ) {
    QString SQL = QString("select delType( %1 );").arg( id );

    CVDbResult* res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch( pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return -1;
    }

    if( res == nullptr || res->getRowCount() != 1) {
        if( res )
            delete res;
        return -1;
    }
    long long idd = res->getCellAsInt64(0, 0);

    delete res;
    return idd;
}

long long dbWriter::insertAircraftImage( shared_ptr< AircraftImage > aircImage ) {
    long long idImage = insertImage( aircImage->getImage(), aircImage->getName() );
    if( aircImage->getType() == nullptr )
        return idImage;
    QString SQL = QString("select set_image_type( %1, %2 );").arg( idImage ).arg( aircImage->getType()->getId() );
    CVDbResult* res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch( pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return -1;
    }

    if( res == nullptr || res->getRowCount() != 1) {
        if( res )
            delete res;
        return -1;
    }
    long long idd = res->getCellAsInt64(0, 0);

    delete res;
    return idImage;

}

long long dbWriter::updateAircraftImage( shared_ptr< AircraftImage > aircImage ) {
    if( aircImage == nullptr )
        return -1;
    long long idImage = updateImage( aircImage->getImage(), aircImage->getName(), aircImage->getId() );
    QString SQL = QString("select set_image_type( %1, %2 );").arg( idImage )
                                                             .arg( aircImage->getType() ? QString::number (aircImage->getType()->getId()) : QString("NULL::bigint" ) );
    CVDbResult* res = nullptr;
    try {
        res = m_db->execute( SQL.toStdString().c_str() );
    }
    catch( pqxx::failure& e) {
        qDebug() << __PRETTY_FUNCTION__ << e.what();
        if( res )
            delete res;
        return -1;
    }

    if( res == nullptr || res->getRowCount() != 1) {
        if( res )
            delete res;
        return -1;
    }
    long long idd = res->getCellAsInt64(0, 0);

    delete res;
    return idImage;
}
