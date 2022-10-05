#pragma once

#include <string>
#include <vector>
#include <variant>

#include "opencv_db_result.h"

#include "opencv_database.h"

class OpenCVPgDatabase : public OpenCVDatabase {
public:
    OpenCVPgDatabase();
    OpenCVPgDatabase( const OpenCVPgDatabase& ) = delete;
    OpenCVPgDatabase& operator=( const OpenCVPgDatabase& ) = delete;
    virtual ~OpenCVPgDatabase();

    virtual bool connect( string _ipServer = string(),
                          string _database = string(),
                          string _user = string(),
                          string _password = string(), 
                          int _port = 5432,
                          bool reconnect = false ) const override;

    virtual bool connect( bool reconnect = false ) const override;
    virtual void disconnect( bool reconnect = false ) const override;
    virtual bool connected() const override;

    virtual CVDbResult * execute( const char* query ) const override;
    virtual CVDbResult * execSQL( const char * sql, ...) const override;

    virtual CVDbResult * execPrepared(
                                    const char* stmtName, 
                                    int nParams, 
                                    const char * const * paramValues, 
                                    const int * paramLengths, 
                                    const int * paramFormats, 
                                    int resultFormat
                                   ) const override;


    virtual CVDbResult * execParams(
                                    const char* command, 
                                    int nParams, 
                                    const int * paramTypes, 
                                    const char * const * paramValues, 
                                    const int * paramLengths, 
                                    const int * paramFormats, 
                                    int resultFormat
                                   ) const override;
    virtual string   escapeAsciiString(const char * fromString) const override;
    virtual string   escapeBinaryString(const unsigned char * fromString) const override;

    virtual bool begin() const override;
    virtual bool commit() const override;
    virtual bool rollback() const override;

private:
    mutable pqxx::connection* _dbConnection;
    mutable pqxx::work* _dbWork;
};
