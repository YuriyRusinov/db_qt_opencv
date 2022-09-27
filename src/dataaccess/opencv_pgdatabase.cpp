#include <iostream>
#include <sstream>

#include <pqxx/pqxx>
#include <pqxx/connection.hxx>
#include "opencv_pgdatabase.h"

using std::cout;
using std::endl;
using std::stringstream;
using pqxx::connection;

OpenCVPgDatabase::OpenCVPgDatabase()
    : _dbConnection ( nullptr ),
    _dbWork ( nullptr ) {
}

OpenCVPgDatabase::~OpenCVPgDatabase() {}

bool OpenCVPgDatabase::connect( string _ipServer,
                                string _database,
                                string _user,
                                string _password, 
                                int _port,
                                bool reconnect ) const {
    stringstream connString;
    connString << " user= " << _user << " password= " << _password << " server=" << _ipServer << " dbname=" << _database << " port=" << _port;
    if( _dbConnection != nullptr )
        delete _dbConnection;

    bool retVal = true;
    try {
        _dbConnection = new pqxx::connection( connString.str() );
        retVal = _dbConnection->is_open();
    }
    catch( const std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return retVal;
}

bool OpenCVPgDatabase::connect( bool reconnect ) const {
    return false;
}

void OpenCVPgDatabase::disconnect( bool reconnect ) const {
    return ;
}

bool OpenCVPgDatabase::connected() const {
    return false;
}

CVDbResult * OpenCVPgDatabase::execute( const char* query ) const {
    return nullptr;
}

CVDbResult * OpenCVPgDatabase::execSQL( const char * sql, ...) const {
    return nullptr;
}

CVDbResult * OpenCVPgDatabase::execPrepared(
                                const char* stmtName, 
                                int nParams, 
                                const char * const * paramValues, 
                                const int * paramLengths, 
                                const int * paramFormats, 
                                int resultFormat
                               ) const 
{
    return nullptr;
}

CVDbResult * OpenCVPgDatabase::execParams(
                                const char* command, 
                                int nParams, 
                                const int * paramTypes, 
                                const char * const * paramValues, 
                                const int * paramLengths, 
                                const int * paramFormats, 
                                int resultFormat
                               ) const 
{
    return nullptr;
}

char * OpenCVPgDatabase::escapeAsciiString(const char * fromString) const {
    return nullptr;
}

string OpenCVPgDatabase::escapeBinaryString(const unsigned char * fromString) const {
    return string();
}

bool OpenCVPgDatabase::begin() const {
    return false;
}

bool OpenCVPgDatabase::commit() const {
    return false;
}

bool OpenCVPgDatabase::rollback() const {
    return false;
}
