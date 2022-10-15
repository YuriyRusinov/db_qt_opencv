#pragma once

#include <string>
#include <list>
#include <vector>
#include <variant>

#include "opencv_db_result.h"

using std::string;
using std::list;
using std::vector;
using std::variant;

namespace pqxx {
    class result;
};

class CVDbPgResult : public CVDbResult {
public:
    CVDbPgResult(pqxx::result* res) : CVDbResult(), m_res(res) {};
    CVDbPgResult( const CVDbPgResult& dbr ) = delete;
    CVDbPgResult& operator=( const CVDbPgResult& dbr ) = delete;
    virtual ~CVDbPgResult();

    virtual CVDbResult::DataType getColumnDataType(int column) const override;
    virtual int getColumnNumber(const char * columnName) const override;
    virtual const char * getColumnName(int column) const override;

    virtual const char * getCellData( int row, int column ) const override; // возвращает результат запроса в виде const char *
    virtual int getCellLength( int row, int column ) const override;
    
    virtual string getCell(int row, int column) const override; // Возвращает результат sql-запроса в формате QVariant
    virtual QByteArray getCellAsByteArray (int row, int column) const override; // Возвращает результат sql-запроса в виде QByteArray, удобно для полей типа bytea
    virtual pqxx::binarystring getCellAsBinaryString( int row, int column) const override;

    virtual bool isEmpty(int row, int column) const override;
    virtual int getRowCount() const override;
    virtual int getColumnCount() const override;
    virtual CVDbResult::ResultStatus resultStatus() const override;
    virtual string errorMessage() const override;
    virtual string errorCode() const override;

    virtual void next() override;
    virtual void prev() override;

private:
    pqxx::result* m_res;
    int rowCount;
    int columnCount;
    
    enum PGDataType{
        pgGEOMETRY = 0,

        pgBOOLOID    = 16,
        pgINT2OID    = 21,
        pgINT4OID    = 23,
        pgINT8OID    = 20,
        pgFLOAT4OID  = 700,
        pgFLOAT8OID  = 701,
        pgCHAROID    = 18,
        pgNAMEOID    = 19,
        pgTEXTOID    = 25,
        pgVARCHAROID = 1043,
        pgBYTEAOID   = 17,
        pgTIMESTAMPOID = 1114,

        pgBOOLARRAYOID    = 1000,
        pgINT2ARRAYOID    = 1005,
        pgINT4ARRAYOID    = 1007,
        pgINT8ARRAYOID    = 1016,
        pgFLOAT4ARRAYOID  = 1021,
        pgFLOAT8ARRAYOID  = 1022,

        pgCHARARRAYOID    = 1002,
        pgNAMEARRAYOID    = 1003,
        pgTEXTARRAYOID    = 1009,
        pgVARCHARARRAYOID = 1015,
        pgBYTEAARRAYOID   = 1001,
        pgTIMESTAMPARRAYOID = 1115
    };
};
