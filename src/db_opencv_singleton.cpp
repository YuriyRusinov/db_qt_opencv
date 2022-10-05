#include <iostream>

#include <dbLoader.h>
#include <opencvcore.h>
#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include "db_opencv_singleton.h"

using std::make_unique;

dbOpenCvS* dbOpenCvS::_instance = nullptr;

dbOpenCvS* dbOpenCvS::getDBOpenCVSingleton() {
    if( _instance == nullptr ) {
        _instance = new dbOpenCvS;
    }
    return _instance;
}

void dbOpenCvS::reset() {
    bool isInst ( _instance !=nullptr );
    //std::cout << __PRETTY_FUNCTION__ << ' ' << isInst << std::endl;
    if( _instance != nullptr ) {
        delete _instance;
        std::cout << __PRETTY_FUNCTION__ << "Deleted " << std::endl;
    }
    _instance = nullptr;
}

dbOpenCvS::dbOpenCvS()
    : m_db( new OpenCVPgDatabase ),
    m_cvcore( new OpenCVCore(m_db) ) {
}

dbOpenCvS::~dbOpenCvS() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    delete m_cvcore;
    delete m_db;
}
