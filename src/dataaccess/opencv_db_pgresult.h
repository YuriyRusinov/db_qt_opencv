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

class CVDbPgResult : public CVDbResult {
public:
    CVDbPgResult() : CVDbResult() {};
    CVDbPgResult( const CVDbPgResult& dbr ) = delete;
    CVDbPgResult& operator=( const CVDbPgResult& dbr ) = delete;
    virtual ~CVDbPgResult() {};

    virtual CVDbResult::DataType getColumnDataType(int column) const override;
    virtual int getColumnNumber(const char * columnName) const override;
    virtual const char * getColumnName(int column) const override;

    virtual unsigned char * getCellData( int row, int column ) const override; // возвращает результат запроса в виде char *
    virtual int getCellLength( int row, int column ) const override;
    
    virtual string getCell(int row, int column) const override; // Возвращает результат sql-запроса в формате QVariant
    virtual vector< char > getCellAsByteArray (int row, int column) const override; // Возвращает результат sql-запроса в виде QByteArray, удобно для полей типа bytea

    virtual bool isEmpty(int row, int column) const override;
    virtual int getRowCount() const override;
    virtual int getColumnCount() const override;
    virtual CVDbResult::ResultStatus resultStatus() const override;
    virtual string errorMessage() const override;
    virtual string errorCode() const override;

    virtual void next() override;
    virtual void prev() override;
};
