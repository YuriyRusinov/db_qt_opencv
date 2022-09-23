#pragma once

class dbOpenCvS {
public:
    static dbOpenCvS* getDBOpenCVSingleton();
    static void reset();
private:
    dbOpenCvS();
    dbOpenCvS( const dbOpenCvS& ) = delete;
    dbOpenCvS& operator=( const dbOpenCvS& ) = delete;
    virtual ~dbOpenCvS();

    static dbOpenCvS* _instance;
};
