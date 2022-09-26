#include <dbLoader.h>
#include <opencvcore.h>
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
    if( _instance )
        delete _instance;
    _instance = nullptr;
}

dbOpenCvS::dbOpenCvS()
    : m_cvcore( new OpenCVCore ) {
    //dbLoader* dbW = new dbLoader;
    m_databaseLoader = make_unique<dbLoader>( new dbLoader );
}

dbOpenCvS::~dbOpenCvS() {
}

unique_ptr<dbLoader>& dbOpenCvS::getDb() const {
    return m_databaseLoader;
}
