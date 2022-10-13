/*
#include <QBuffer>
#include <QFile>
#include <QDataStream>
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include <pqxx/result.hxx>
#include <pqxx/row.hxx>
#include <pqxx/field.hxx>
#include <pqxx/binarystring.hxx>

#include "opencv_db_pgresult.h"

using std::vector;

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
    pqxx::binarystring res_pqxx ( fCell );
    return res_pqxx.get();
} // возвращает результат запроса в виде const char *

int CVDbPgResult::getCellLength( int row, int column ) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return -1;
    pqxx::field fCell( m_res->at(row).at(column) );
    pqxx::binarystring res_pqxx ( fCell );
    return res_pqxx.size();
}

string CVDbPgResult::getCell(int row, int column) const {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return string();

    pqxx::field fCell( m_res->at(row).at(column) );
    pqxx::binarystring fCellB( fCell );
    std::cerr << fCellB.data() << std::endl;//m_res->at(row).at(column);
    return pqxx::to_string(fCell);
} // Возвращает результат sql-запроса в формате std::string

QByteArray CVDbPgResult::getCellAsByteArray (int row, int column) const {
    if( m_res == nullptr || row >= m_res->size() || column >= m_res->at(row).size() )
        return QByteArray();

    pqxx::field fCell( m_res->at(row).at(column) );
    pqxx::binarystring fCellB( fCell );
    pqxx::field::size_type nn = fCellB.size();//strlen((const char*)buffer);
    QByteArray resbytes = QByteArray::fromRawData( fCellB.get(), nn);
/*  
    for Debug purposes

    QFile fByteArr("ttt_by_pg.bin");
    fByteArr.open( QBuffer::WriteOnly );
    QDataStream fByteArrStr( &fByteArr );
    fByteArrStr << resbytes;
*/
    return resbytes;
} // Возвращает результат sql-запроса в виде QByteArray, удобно для полей типа bytea

bool CVDbPgResult::isEmpty(int row, int column) const {
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

