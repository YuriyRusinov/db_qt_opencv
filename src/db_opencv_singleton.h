#pragma once

#include <memory>

using std::unique_ptr;

class dbLoader;
class OpenCVCore;
class OpenCVDatabase;

class dbOpenCvS {
public:
    static dbOpenCvS* getDBOpenCVSingleton();
    static void reset();

    OpenCVCore* getCore() const { return m_cvcore; }

private:
    dbOpenCvS();
    dbOpenCvS( const dbOpenCvS& ) = delete;
    dbOpenCvS& operator=( const dbOpenCvS& ) = delete;
    virtual ~dbOpenCvS();

    mutable OpenCVDatabase* m_db;
    mutable OpenCVCore* m_cvcore;

    static dbOpenCvS* _instance;
};
