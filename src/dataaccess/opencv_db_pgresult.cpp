
#include <QBuffer>
#include <QFile>
#include <QDataStream>
#include <QtDebug>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>

#include <pqxx/result>
#include <pqxx/row>
#include <pqxx/field>
#include <pqxx/binarystring>
#include <opencv2/imgcodecs.hpp>
#include "opencv_db_pgresult.h"

using std::vector;
using std::string;
using std::basic_string;
using std::byte;

CVDbPgResult::~CVDbPgResult() {
    if( m_res )
        delete m_res;
}

CVDbResult::DataType CVDbPgResult::getColumnDataType(int column) const
{
    if( m_res == nullptr || m_res->size() == 0 || column >= m_res->at(0).size() )
        return CVDbResult::dtUnknown;
    pqxx::field fCell ( m_res->at(0).at(column) );
    return (CVDbResult::DataType)fCell.type();
}

int CVDbPgResult::getColumnNumber(const char * columnName) const
{
    if( m_res == nullptr || columnName == nullptr || m_res->size() == 0 || m_res->at(0).size()==0 )
        return -1;

    pqxx::row r = m_res->at(0);
    return r.column_type( columnName );
}

const char * CVDbPgResult::getColumnName(int column) const {
    if( m_res == nullptr || m_res->size() == 0 || column >= m_res->at(0).size() )
        return nullptr;

    const char* cName = m_res->column_name(column);
    return cName;
}

const char * CVDbPgResult::getCellData( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return nullptr;

    pqxx::field fCell( m_res->at(row).at(column) );
#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring res_pqxx ( fCell );
    return res_pqxx.get();
#else
    int nn = fCell.size();
    const char* res_s = fCell.c_str();//resc.value();
    return res_s;
#endif
    //qDebug() << __PRETTY_FUNCTION__ << strncmp(res_pqxx.get(), res_s, nn ) << isSaved << res_pqxx.size() << nn << strlen(res_pqxx.get());
} // возвращает результат запроса в виде const char *

int CVDbPgResult::getCellLength( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return -1;

    pqxx::field fCell( m_res->at(row).at(column) );
#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring res_pqxx ( fCell );
    return res_pqxx.size();
#else
    return fCell.size();
#endif
}

string CVDbPgResult::getCell(int row, int column) const {
    //std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return string();

    pqxx::field fCell( m_res->at(row).at(column) );
    return pqxx::to_string(fCell);
} // Возвращает результат sql-запроса в формате std::string

QByteArray CVDbPgResult::getCellAsByteArray( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return QByteArray();

    pqxx::field fCell( m_res->at(row).at(column) );
    int nn = getCellLength( row, column );//fCell.size();
#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring fCellB = getCellAsBinaryString0( row, column );
    pqxx::binarystring fCellBB( fCell );
    qDebug() << __PRETTY_FUNCTION__ << (fCellB == fCellBB) << row << column;
    const char* imBytes = fCell.get();//c_str();
#else
    const void* imV = getCellAsPointer( row, column );
    const char* imBytes = static_cast<const char *>(imV);//(imBytesStr.data());
#endif
    QByteArray resbytes = QByteArray::fromRawData( imBytes, nn);
    return resbytes;
} // Возвращает результат sql-запроса в виде QByteArray, удобно для полей типа bytea

QImage CVDbPgResult::getCellAsImage( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return QImage();

    int nn = getCellLength( row, column );
#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring imBytesStr0 = getCellAsBinaryString0( row, column );
    const char* imBytes = imBytesStr0.get();
#else
    basic_string<std::byte> imBytesStr = getCellAsBinaryString( row, column );
    const void* imV = getCellAsPointer( row, column );//static_cast<const void*>(imBytesStr.data());
    basic_string<std::byte> imBytesStr0 = getCellAsBinaryString( row, column );
    const void* imV0 = static_cast<const void*>(imBytesStr.data());//getCellAsPointer( row, column );
    qDebug() << __PRETTY_FUNCTION__ << memcmp(imV, imV0, nn) << imV << imV0;
    const char* imBytes = static_cast<const char *>(imV);
#endif
    QByteArray imABytes = QByteArray::fromRawData( imBytes, nn );
    QByteArray imABytes0 = getCellAsByteArray( row, column );
    QImage resImage;
    bool isLoaded = resImage.loadFromData( imABytes );
    QImage tIm;
    bool isLoaded0 = tIm.loadFromData( imABytes0 );
    qDebug() << __PRETTY_FUNCTION__ << isLoaded << isLoaded0 << (resImage == tIm ) << imABytes.compare(imABytes0);
    return resImage;
}

cv::Mat CVDbPgResult::getCellAsMatrix( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return cv::Mat();

#if PQXX_VERSION_MAJOR < 7
    pqxx::binarystring imBytesStr = getCellAsBinaryString( row, column );
    const char* imBytes = imBytesStr.get();
#else
    basic_string<std::byte> imBytesStr = getCellAsBinaryString( row, column );
    const char* imBytes = static_cast<const char *>(static_cast<const void *>(imBytesStr.data()));
#endif
    int nn = getCellLength( row, column );
    vector<uchar> jpgbytes(imBytes, imBytes+nn);
    cv::Mat res = cv::imdecode(jpgbytes, cv::IMREAD_COLOR);//cv::CV_LOAD_IMAGE_COLOR);
    return res;
}

#if PQXX_VERSION_MAJOR < 7
pqxx::binarystring CVDbPgResult::getCellAsBinaryString0( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return pqxx::binarystring(nullptr, 0);

    pqxx::field fCell( m_res->at(row).at(column) );
    pqxx::binarystring fCellB( fCell );
    return fCellB;
}
#else
std::basic_string<std::byte> CVDbPgResult::getCellAsBinaryString( int row, int column) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return std::basic_string<std::byte>(nullptr, 0);

    pqxx::field fCell( m_res->at(row).at(column) );
    return fCell.as< std::basic_string<std::byte> >();
}

const void* CVDbPgResult::getCellAsPointer( int row, int column) const {
    std::basic_string<std::byte> bytes = getCellAsBinaryString(row, column);
    int nn = bytes.size();
    void* pRes = new char[nn+1];
    memcpy(pRes, static_cast<const void*>(bytes.data()), nn);
    return pRes;
}
#endif

bool CVDbPgResult::isEmpty( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return true;

    pqxx::field fCell( m_res->at(row).at(column) );
    return fCell.is_null();
}

int CVDbPgResult::getRowCount() const {
    if( m_res == nullptr )
        return -1;
    return m_res->size();
}

int CVDbPgResult::getColumnCount() const {
    if( m_res == nullptr || m_res->size() == 0 )
        return -1;

    pqxx::row r = m_res->at(0);
    return m_res->at(0).size();
}

CVDbResult::ResultStatus CVDbPgResult::resultStatus() const {
    if( m_res == nullptr )
        return CVDbResult::Unknown;

    return CVDbResult::CommandOk;
}

string CVDbPgResult::errorMessage() const {
    return string();
}

string CVDbPgResult::errorCode() const {
    return string();
}

void CVDbPgResult::next() {
}

void CVDbPgResult::prev() {
}

