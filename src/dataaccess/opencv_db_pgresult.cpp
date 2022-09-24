#include "opencv_db_pgresult.h"

using std::vector;

CVDbResult::DataType CVDbPgResult::getColumnDataType(int column) const
{
    return CVDbResult::dtUnknown;
}

int CVDbPgResult::getColumnNumber(const char * columnName) const
{
    return -1;
}

const char * CVDbPgResult::getColumnName(int column) const {
    return nullptr;
}

unsigned char * CVDbPgResult::getCellData( int row, int column ) const {
    return nullptr;
} // возвращает результат запроса в виде char *

int CVDbPgResult::getCellLength( int row, int column ) const {
    return -1;
}

string CVDbPgResult::getCell(int row, int column) const {
    return string();
} // Возвращает результат sql-запроса в формате QVariant

vector< char > CVDbPgResult::getCellAsByteArray (int row, int column) const {
    return vector<char>();
} // Возвращает результат sql-запроса в виде QByteArray, удобно для полей типа bytea

bool CVDbPgResult::isEmpty(int row, int column) const {
    return true;
}

int CVDbPgResult::getRowCount() const {
    return -1;
}

int CVDbPgResult::getColumnCount() const {
    return -1;
}

CVDbResult::ResultStatus CVDbPgResult::resultStatus() const {
    return CVDbResult::Unknown;
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

