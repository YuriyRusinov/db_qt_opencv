#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>

#include <pqxx/pqxx>
#include <pqxx/connection>
#include <pqxx/transaction>
#include <pqxx/result>
#include <pqxx/version>

#include "opencv_db_pgresult.h"
#include "opencv_pgdatabase.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::ios_base;
using pqxx::connection;

OpenCVPgDatabase::OpenCVPgDatabase()
    : _dbConnection ( nullptr ),
    _dbWork ( nullptr ), 
    _prepCounter( new int(0) ) {
}

OpenCVPgDatabase::~OpenCVPgDatabase() {
    delete _prepCounter;
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
    connString << " user= " << _user << " password= " << _password << " host=" << _ipServer << " dbname=" << _database << " port=" << _port;
    if( _dbConnection != nullptr )
        delete _dbConnection;

    bool retVal = true;
    try {
        _dbConnection = new pqxx::connection( connString.str() );
        retVal = _dbConnection->is_open();
    }
    catch( const std::exception &e ) {
        std::cerr << __PRETTY_FUNCTION__ << ' ' << e.what() << std::endl;
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
    if( res == nullptr ) {
        delete _dbWork;
        return nullptr;
    }
    CVDbResult * pRes = new CVDbPgResult ( res );
    _dbWork->commit();

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
    (*_prepCounter)++;
    stringstream eStrPC( "execParams", ios_base::app | ios_base::out );
    eStrPC << *_prepCounter;
    string eStr = eStrPC.str();
    _dbConnection->prepare(eStr, SQLstr.str().c_str());
    if( _dbWork )
        delete _dbWork;
    _dbWork = new pqxx::work( *_dbConnection );
#if PQXX_VERSION_MAJOR < 7
    std::vector< pqxx::binarystring > vparams;
#else
    std::vector< std::string > vparams;
#endif
    for(int i=0; i<nParams; i++) {
        switch( paramTypes[i] ) {
            case CVDbResult::DataType::dtInt8 :
            case CVDbResult::DataType::dtInt4 :
            case CVDbResult::DataType::dtInt2 : {
               long long idValue (atoll( paramValues[i] ));
               stringstream vStr;
               vStr << idValue;
#if PQXX_VERSION_MAJOR < 7
                pqxx::binarystring vstr ( pqxx::to_string(idValue) );
#else
                const void* mStr = static_cast<const void *>( vStr.str().c_str() );
                int n = vStr.str().size();
                std::string vstr( vStr.str() );
#endif
                string s = ( vStr.str() );
                cerr << __PRETTY_FUNCTION__ << s << ' ' << idValue << endl;
                vparams.push_back( vstr );
                break;
            }
            case CVDbResult::DataType::dtVarchar : default: {
                char* paramData = new char [paramLengths[i]];
                strncpy( paramData, paramValues[i], paramLengths[i]);
                cerr << __PRETTY_FUNCTION__ << ' ' << paramData << endl;
                string esc_str = _dbConnection->esc(paramData);
                cerr << __PRETTY_FUNCTION__ << ' ' << esc_str << endl;
#if PQXX_VERSION_MAJOR < 7
                pqxx::binarystring vchar( esc_str.c_str(), paramLengths[i]);
#else
                string pStr( paramValues[i], paramLengths[i]);
                const void* mStr = (const void *)paramValues[i];
                std::string vchar( pStr );
                cerr << __PRETTY_FUNCTION__ << ' ' << vchar;
#endif
                vparams.push_back( vchar );
                delete [] paramData;
                break;
            }
            case CVDbResult::DataType::dtBytea : {
                void* paramData = (void *)paramValues[i];
                int paramSize = paramLengths[i];
#if PQXX_VERSION_MAJOR < 7
                pqxx::binarystring blob(paramData, paramSize);
#else
                const void* blobV = static_cast<const void *>(paramData);
                std::basic_string_view<std::byte> blob0( static_cast<const std::byte*>(blobV), paramSize);
                string blob = _dbConnection->esc_raw( blob0 );
#endif
                vparams.push_back( blob );
                break;
            }
        }
    }
    pqxx::result *res = nullptr;
#if PQXX_VERSION_CHECK < 0x070600
    try {
        res = new pqxx::result (_dbWork->exec_prepared(eStr, pqxx::prepare::make_dynamic_params(vparams)));
    }
#else
    pqxx::params vpars( vparams );
    try {
        res = new pqxx::result (_dbWork->exec_prepared(eStr, vpars) );
    }
#endif
    catch( pqxx::failure& e) {
        cerr << __PRETTY_FUNCTION__ << ' ' << e.what();
        this->rollback();
        return nullptr;
    }

    if (std::empty(*res)) {
        this->rollback();
        throw std::runtime_error{"query does not executed!"};
    }
    _dbWork->commit();
    CVDbResult* rParamRes = new CVDbPgResult( res );
    delete _dbWork;
    _dbWork = nullptr;
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
    //string fStr( (char *)fromString, n);
#if PQXX_VERSION_MAJOR < 7
    return _dbConnection->esc_raw( fromString, n );
#else
    const void* mStr = static_cast<const void *>( fromString );
    std::basic_string<std::byte> bFromString( static_cast<const std::byte *>(mStr), n);
    return _dbConnection->esc_raw( bFromString );
#endif
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
