#include <opencv_database.h>
#include <opencv_pgdatabase.h>
#include "dbLoader.h"

dbLoader::dbLoader( QObject* parent )
    : QObject( parent ), 
    m_db( new OpenCVPgDatabase ) {
}

dbLoader::~dbLoader() {

}
