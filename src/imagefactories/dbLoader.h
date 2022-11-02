#pragma once

#include <QMap>
#include <QObject>

#include <map>
#include <memory>

#include <AircraftImage.h>
#include <AircraftType.h>

using std::shared_ptr;
using std::make_shared;
using std::map;

class OpenCVDatabase;

class dbLoader : public QObject {
public:
    explicit dbLoader( OpenCVDatabase* db, QObject* parent = nullptr );
    dbLoader() = delete;
    dbLoader( const dbLoader& ) = delete;
    dbLoader& operator=( const dbLoader& dbl ) = delete;
    virtual ~dbLoader();

    QMap< long long, shared_ptr< AircraftImages > > loadImages() const;
    shared_ptr< AircraftImages > loadImage( qlonglong id ) const;

    map< long long, shared_ptr< AircraftType > > loadTypes() const;

    friend class dbOpenCvS;
private:
    OpenCVDatabase* m_db;

private:
    Q_OBJECT
};
