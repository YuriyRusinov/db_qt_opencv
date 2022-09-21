#include <sstream>
#include "opencv_db_result.h"

using std::stringstream;

int CVDbResult::getCellAsInt(int row, int column, bool * ok) const {
    string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return 0;
    }
    stringstream vs(v);
    int x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
} // Возвращает результат sql-запроса в виде целого числа

long long CVDbResult::getCellAsInt64(int row, int column, bool * ok) const {
    string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return 0;
    }
    stringstream vs(v);
    long long x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
}
// Возвращает результат sql-запроса в виде целого числа

bool CVDbResult::getCellAsBool(int row, int column, bool * ok) const {
    string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return false;
    }
    stringstream vs(v);
    bool x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
}// Возвращает результат sql-запроса в булевом виде

string CVDbResult::getCellAsString(int row, int column, bool * ok) const {\
    string v = getCell(row, column);
    if(ok)
        *ok = true;
    return v;
}// строка

double CVDbResult::getCellAsDouble(int row, int column, bool * ok) const {
    string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return 0;
    }
    stringstream vs(v);
    double x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
}// число двойной точности

vector< string > CVDbResult::getCellAsStringList(int row, int column, bool * ok) const {
    string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return vector< string >();
    }
    stringstream vs(v);
    char delim = ' ';
    vector< string > res;
    while( !vs.eof() ) {
        string w;
        getline(vs, w, delim);
        res.push_back( w );
    }
    return res;
}// список строк

string CVDbResult::getCellAsDateTime(int row, int column, bool * ok) const {
    string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return string();
    }
    return v;
    //stringstream vs(v);
}// date and time in ISO format 

