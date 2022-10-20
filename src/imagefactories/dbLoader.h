#pragma once

#include <QMap>
#include <QObject>
#include <memory>

#include "dbImageClass.h"

using std::shared_ptr;
using std::make_shared;

class OpenCVDatabase;
class dbLoader : public QObject {
public:
    explicit dbLoader( OpenCVDatabase* db, QObject* parent = nullptr );
    dbLoader() = delete;
    dbLoader( const dbLoader& ) = delete;
    dbLoader& operator=( const dbLoader& dbl ) = delete;
    virtual ~dbLoader();

    QMap< long long, shared_ptr< dbImages > > loadImages() const;
    shared_ptr< dbImages > loadImage( qlonglong id ) const;

    friend class dbOpenCvS;
private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
