#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <variant>

#include "opencv_db_result.h"

using std::string;
using std::vector;
using std::variant;

class OpenCVDatabase {
public:
    OpenCVDatabase();
    OpenCVDatabase( const OpenCVDatabase& db ) = delete;
    OpenCVDatabase& operator=( const OpenCVDatabase& db ) = delete;
    virtual ~OpenCVDatabase();

    virtual bool connect( string _ipServer = string(),
                          string _database = string(),
                          string _user = string(),
                          string _password = string(), 
                          string _port = "5432",
                          bool reconnect = false ) const = 0;

    virtual bool connect( bool reconnect ) const = 0;
    virtual void disconnect( bool reconnect ) const = 0;
    virtual bool connected() const = 0;

    virtual CVDbResult * execute( const char* query ) const = 0;
    CVDbResult * execute( const string & query) const;
    int executeCmd( const string & sql ) const;
    virtual CVDbResult * execSQL( const char * sql, ...) const = 0;

    virtual CVDbResult * execPrepared(
                                    const char* stmtName, 
                                    int nParams, 
                                    const char * const * paramValues, 
                                    const int * paramLengths, 
                                    const int * paramFormats, 
                                    int resultFormat
                                   ) const = 0;


    virtual CVDbResult * execParams(
                                    const char* command, 
                                    int nParams, 
                                    const int * paramTypes, 
                                    const char * const * paramValues, 
                                    const int * paramLengths, 
                                    const int * paramFormats, 
                                    int resultFormat
                                   ) const = 0;
    CVDbResult * execParams(
                                    const string & command, 
                                    const vector<CVDbResult::DataType>& paramTypes, 
                                    const vector<string> & paramValues,
                                    const vector<int> & paramFormats, 
                                    int resultFormat
                                    ) const;

    virtual char *    escapeAsciiString(const char * fromString) const = 0;
    virtual string   escapeBinaryString(const unsigned char * fromString) const = 0;

    virtual bool begin() const = 0;
    virtual bool commit() const = 0;
    virtual bool rollback() const = 0;

    string getHost() const { return m_ipServer; }
    string getPort() const { return m_port; }
    string getDb() const { return m_database; }
    string getUser() const { return m_user; }
    string getPassword() const { return m_password; }

protected:
    mutable string m_ipServer;
    mutable string m_port;
    mutable string m_database;
    mutable string m_user;
    mutable string m_password;
};
