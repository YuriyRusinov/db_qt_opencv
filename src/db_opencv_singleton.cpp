#include "db_opencv_singleton.h"

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

dbOpenCvS::dbOpenCvS() {
}

dbOpenCvS::~dbOpenCvS() {
}
