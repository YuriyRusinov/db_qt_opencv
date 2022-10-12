#include <iostream>
#include <sstream>

#include <pqxx/pqxx>
#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>
#include <pqxx/result.hxx>

#include "opencv_db_pgresult.h"
#include "opencv_pgdatabase.h"

using std::cout;
using std::cerr;
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
    if( _dbConnection == nullptr )
        return nullptr;

    stringstream SQLstr;
    SQLstr << command << ' ' ;
    for(int i=0; i<nParams; i++) {
        SQLstr << "$" << (i+1) << (i<nParams-1 ? ", " : ");");
    }
    cerr << __PRETTY_FUNCTION__ << " SQL query is " << SQLstr.str() << endl;
    _dbConnection->prepare("execParams", SQLstr.str().c_str());
    if( _dbWork )
        delete _dbWork;
    _dbWork = new pqxx::work( *_dbConnection );
    std::vector< pqxx::binarystring > params;
    for(int i=0; i<nParams; i++) {
        switch( paramTypes[i] ) {
            case CVDbResult::DataType::dtVarchar : default: {
                char* paramData = new char [paramLengths[i]];
                strncpy( paramData, paramValues[i], paramLengths[i]);
                pqxx::binarystring vchar( paramData, paramLengths[i]);
                params.push_back( vchar );
                delete [] paramData;
                break;
            }
            case CVDbResult::DataType::dtBytea : {
                void* paramData = (void *)paramValues[i];
                int paramSize = paramLengths[i];
                //QImage im;
                //QByteArray ba = QByteArray::fromRawData( paramValues[i], paramSize);
                //bool isLoaded = im.loadFromData( ba );
                //cerr << __PRETTY_FUNCTION__ << " " << paramSize << ' ' << (isLoaded ? "true" : "false") << ' ' << endl;
                pqxx::binarystring blob(paramData, paramSize);
                params.push_back( blob );
                break;
            }
        }
    }
    pqxx::result *res = new pqxx::result (_dbWork->exec_prepared("execParams", pqxx::prepare::make_dynamic_params(params)));
    if (std::empty(*res)) {
        this->rollback();
        throw std::runtime_error{"query does not executed!"};
    }
    _dbWork->commit();
    CVDbResult* rParamRes = new CVDbPgResult( res );
    return rParamRes;
}

string OpenCVPgDatabase::escapeAsciiString(const char * fromString) const {
    if(fromString == nullptr || _dbConnection == nullptr)
        return string();

    return _dbConnection->esc( fromString );
}

string OpenCVPgDatabase::escapeBinaryString(const unsigned char * fromString) const {
    if(fromString == nullptr || _dbConnection == nullptr)
        return string();

    int n = strlen( (char *)fromString );
    return _dbConnection->esc_raw( fromString, n );
}

bool OpenCVPgDatabase::begin() const {
    if( _dbConnection == nullptr || _dbWork != nullptr )
        return false;

    _dbWork = new pqxx::work( *_dbConnection );
    return true;
}

bool OpenCVPgDatabase::commit() const {
    if( _dbConnection == nullptr || _dbWork == nullptr )
        return false;

    _dbWork->commit();
    return true;
}

bool OpenCVPgDatabase::rollback() const {
    if( _dbConnection == nullptr || _dbWork == nullptr )
        return false;

    delete _dbWork;
    _dbWork = nullptr;
    return true;
}
