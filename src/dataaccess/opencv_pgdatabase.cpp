#include <iostream>
#include <sstream>

#include <pqxx/pqxx>
#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>
#include <pqxx/result.hxx>

#include "opencv_db_pgresult.h"
#include "opencv_pgdatabase.h"

using std::cout;
using std::endl;
using std::stringstream;
using pqxx::connection;

OpenCVPgDatabase::OpenCVPgDatabase()
    : _dbConnection ( nullptr ),
    _dbWork ( nullptr ) {
}

OpenCVPgDatabase::~OpenCVPgDatabase() {
    cout << __PRETTY_FUNCTION__ << endl;
}

bool OpenCVPgDatabase::connect( string _ipServer,
                                string _database,
                                string _user,
                                string _password, 
                                int _port,
                                bool reconnect ) const {
    if( reconnect )
        disconnect();
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
    if( retVal ) {
        m_ipServer = _ipServer;
        m_port = _port;
        m_database = _database;
        m_user = _user;
        m_password = _password;
    }

    return retVal;
}

bool OpenCVPgDatabase::connect( bool reconnect ) const {
    return connect( m_ipServer, m_database, m_user, m_password, m_port, reconnect);
}

void OpenCVPgDatabase::disconnect( bool reconnect ) const {
    if ( _dbConnection ) {
        delete _dbConnection;
        _dbConnection = nullptr;
    }
    return ;
}

bool OpenCVPgDatabase::connected() const {
    return (_dbConnection != nullptr && _dbConnection->is_open() );
}

CVDbResult * OpenCVPgDatabase::execute( const char* query ) const {
    if( !_dbConnection )
        return nullptr;
    if( _dbWork )
        delete _dbWork;
    _dbWork = new pqxx::work( *_dbConnection );
    string sQuery( query );

    pqxx::result* res = new pqxx::result (_dbWork->exec( sQuery ));
    CVDbResult * pRes = new CVDbPgResult ( res );
//    std::cerr << __PRETTY_FUNCTION__ << ' ' << res->size() << ' ' << pRes->getRowCount() << ' ' << pRes->getColumnCount() << std::endl;

    return pRes;
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
