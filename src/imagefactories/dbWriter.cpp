#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QDataStream>
#include <QtDebug>

#include <iostream>
#include <string>

#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include <opencv_db_result.h>

#include "dbWriter.h"

using std::string;
using std::cerr;
using std::endl;

dbWriter::dbWriter( OpenCVDatabase* db, QObject* parent )
    : QObject(parent),
    m_db( db ) {
}

dbWriter::~dbWriter( ) {
}

int dbWriter::insertImage(const QImage& im, QString imName) {
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
    paramValues[0] = new char [imName.length()+1];
    strncpy( const_cast<char *>(paramValues[0]), imName.toStdString().c_str(), imName.size());
    const void* pBa ( (const void *)baImg );
    int imgN = baImg.size();
    pqxx::binarystring pqStr( pBa, imgN );
    paramValues[1] = pqStr.get();
    int* paramLength = new int[nParams];
    paramLength[0] = imName.size();
    paramLength[1] = imgN;
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

    int imId = -1;
    if (res != nullptr ) {
        imId = res->getCellAsInt(0, 0);
        delete res;
    }

    return imId;
}
