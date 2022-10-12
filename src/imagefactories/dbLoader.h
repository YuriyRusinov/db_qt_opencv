#pragma once

#include <QMap>
#include <QObject>

#include "dbImageClass.h"

class OpenCVDatabase;
class dbLoader : public QObject {
public:
    explicit dbLoader( OpenCVDatabase* db, QObject* parent = nullptr );
    dbLoader() = delete;
    dbLoader( const dbLoader& ) = delete;
    dbLoader& operator=( const dbLoader& dbl ) = delete;
    virtual ~dbLoader();

    QMap<long, dbImages> loadImages();

    friend class dbOpenCvS;
private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
