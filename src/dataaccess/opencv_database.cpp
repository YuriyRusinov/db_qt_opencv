#include "opencv_database.h"

OpenCVDatabase::OpenCVDatabase()
    : m_ipServer( string() ),
      m_port( -1 ),
      m_database( string() ),
      m_user( string() ),
      m_password( string() )
{
}

OpenCVDatabase::~OpenCVDatabase() {}

CVDbResult * OpenCVDatabase::execParams(
                                const string & command, 
                                const vector<CVDbResult::DataType>& paramTypes, 
                                const vector<string>& paramValues,
                                const vector<int> & paramFormats, 
                                int resultFormat
                                ) const
{
    return nullptr;
}
