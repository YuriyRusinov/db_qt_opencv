#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include "dbLoader.h"

dbLoader::dbLoader( OpenCVDatabase* db, QObject* parent )
    : QObject( parent ), 
    m_db( db ) {
}

dbLoader::~dbLoader() {

}
